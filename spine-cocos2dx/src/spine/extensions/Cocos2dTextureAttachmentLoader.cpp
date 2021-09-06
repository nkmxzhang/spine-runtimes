//
//  Cocos2dTextureAttachmentLoader.cpp
//  spine-cocos2d-x
//
//  Created by tapiream on 2021/7/19.
//

#include "Cocos2dTextureAttachmentLoader.h"
#include "cocos2d.h"
#include <spine/Extension.h>
#include <spine/AttachmentVertices.h>

USING_NS_CC;
using namespace spine;

static void deleteAttachmentVertices (void* vertices) {
    auto av = (AttachmentVertices *) vertices;
    if(av) delete av;
}

static unsigned short quadTriangles[6] = {0, 1, 2, 2, 3, 0};

static void initRegionAttachment(RegionAttachment* attachment, const Texture2D *texture) {
    if(!attachment || !texture) {
        return;
    }
    float width = texture->getPixelsWide();
    float height = texture->getPixelsHigh();
    attachment->setUVs(0, 0, 1, 1, false);
    attachment->setRegionOffsetX(0);
    attachment->setRegionOffsetY(0);
    attachment->setRegionWidth(width);
    attachment->setRegionHeight(height);
    attachment->setRegionOriginalWidth(width);
    attachment->setRegionOriginalHeight(height);
    attachment->setWidth(width);
    attachment->setHeight(height);
}

static void initMeshAttachment(MeshAttachment* attachment, const Texture2D *texture) {
    if(!attachment || !texture) {
        return;
    }
    float width = texture->getPixelsWide();
    float height = texture->getPixelsHigh();
    attachment->setRegionU(0);
    attachment->setRegionV(0);
    attachment->setRegionU2(1);
    attachment->setRegionV2(1);
    attachment->setRegionRotate(false);
    attachment->setRegionDegrees(0);
    attachment->setRegionOffsetX(0);
    attachment->setRegionOffsetY(0);
    
    attachment->setRegionWidth(width);
    attachment->setRegionHeight(height);
    attachment->setRegionOriginalWidth(width);
    attachment->setRegionOriginalHeight(height);
    attachment->setWidth(width);
    attachment->setHeight(height);
}


static void setRegionAttachmentVertices(RegionAttachment* attachment, Texture2D *texture) {
    if(!attachment || !texture) {
        return;
    }
    
    auto attachmentVertices = new AttachmentVertices(texture, 4, quadTriangles, 6);
    V3F_C4B_T2F* vertices = attachmentVertices->_triangles->verts;
    for (int i = 0, ii = 0; i < 4; ++i, ii += 2) {
        vertices[i].texCoords.u = attachment->getUVs()[ii];
        vertices[i].texCoords.v = attachment->getUVs()[ii + 1];
    }
    attachment->setRendererObject(attachmentVertices, deleteAttachmentVertices);
}

static void setMeshAttachmentVertices(MeshAttachment* attachment, Texture2D *texture) {
    if(!attachment || !texture) {
        return;
    }
    AttachmentVertices* attachmentVertices = new AttachmentVertices(texture,
                                                                    attachment->getWorldVerticesLength() >> 1,
                                                                    attachment->getTriangles().buffer(),
                                                                    attachment->getTriangles().size());
    V3F_C4B_T2F* vertices = attachmentVertices->_triangles->verts;
    for (int i = 0, ii = 0, nn = attachment->getWorldVerticesLength(); ii < nn; ++i, ii += 2) {
        vertices[i].texCoords.u = attachment->getUVs()[ii];
        vertices[i].texCoords.v = attachment->getUVs()[ii + 1];
    }
    attachment->setRendererObject(attachmentVertices, deleteAttachmentVertices);
}

RTTI_IMPL(Cocos2dTextureAttachmentLoader, AttachmentLoader)

Cocos2dTextureAttachmentLoader::Cocos2dTextureAttachmentLoader() : AttachmentLoader() {}

Cocos2dTextureAttachmentLoader::~Cocos2dTextureAttachmentLoader() {}

RegionAttachment *Cocos2dTextureAttachmentLoader::newRegionAttachment(Skin &skin, const String &name, const String &path) {
    SP_UNUSED(skin);
    auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(path.buffer());
    if (!texture) return NULL;
    
    RegionAttachment *attachment = new(__FILE__, __LINE__) RegionAttachment(name);
    attachment->setRendererObject(texture);
    initRegionAttachment(attachment, texture);
    return attachment;
}

MeshAttachment *Cocos2dTextureAttachmentLoader::newMeshAttachment(Skin &skin, const String &name, const String &path) {
    SP_UNUSED(skin);
    auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(path.buffer());
    if (!texture) return NULL;
    
    MeshAttachment *attachment = new(__FILE__, __LINE__) MeshAttachment(name);
    attachment->setRendererObject(texture);
    initMeshAttachment(attachment, texture);
    return attachment;
}

BoundingBoxAttachment *Cocos2dTextureAttachmentLoader::newBoundingBoxAttachment(Skin &skin, const String &name) {
    SP_UNUSED(skin);
    return new(__FILE__, __LINE__) BoundingBoxAttachment(name);
}

PathAttachment *Cocos2dTextureAttachmentLoader::newPathAttachment(Skin &skin, const String &name) {
    SP_UNUSED(skin);
    return new(__FILE__, __LINE__) PathAttachment(name);
}

PointAttachment *Cocos2dTextureAttachmentLoader::newPointAttachment(Skin &skin, const String &name) {
    SP_UNUSED(skin);
    return new(__FILE__, __LINE__) PointAttachment(name);
}

ClippingAttachment *Cocos2dTextureAttachmentLoader::newClippingAttachment(Skin &skin, const String &name) {
    SP_UNUSED(skin);
    return new(__FILE__, __LINE__) ClippingAttachment(name);
}

void Cocos2dTextureAttachmentLoader::configureAttachment(Attachment* attachment) {
    if (attachment->getRTTI().isExactly(RegionAttachment::rtti)) {
        auto regionAttachment = (RegionAttachment*)attachment;
        auto texture = (Texture2D*)regionAttachment->getRendererObject();
        if(!texture) {
            return;
        }
        setRegionAttachmentVertices(regionAttachment, texture);
    } else if (attachment->getRTTI().isExactly(MeshAttachment::rtti)) {
        auto meshAttachment = (MeshAttachment*)attachment;
        auto texture = (Texture2D*)meshAttachment->getRendererObject();
        if(!texture) {
            return;
        }
        setMeshAttachmentVertices(meshAttachment, texture);
    }
}

void Cocos2dTextureAttachmentLoader::refreshAttachment(Attachment* attachment, const String& path) {
    auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(path.buffer());
    if (!texture) return;
    
    if (attachment->getRTTI().isExactly(RegionAttachment::rtti)) {
        auto regionAttachment = (RegionAttachment*)attachment;
        initRegionAttachment(regionAttachment, texture);
        //regionAttachment->updateOffset();
        setRegionAttachmentVertices(regionAttachment, texture);
    } else if (attachment->getRTTI().isExactly(MeshAttachment::rtti)) {
        auto meshAttachment = (MeshAttachment*)attachment;
        initMeshAttachment(meshAttachment, texture);
        meshAttachment->updateUVs();
        setMeshAttachmentVertices(meshAttachment, texture);
    }
}
