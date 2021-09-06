//
//  Cocos2dMultiAtlasAttachmentLoader.cpp
//  spine-cocos2d-x
//
//  Created by tapiream on 2021/7/19.
//

#include "Cocos2dMultiAtlasAttachmentLoader.h"
#include "cocos2d.h"
#include <spine/Extension.h>
#include <spine/AttachmentVertices.h>

USING_NS_CC;
using namespace spine;

static void deleteAttachmentVertices (void* vertices) {
    delete (AttachmentVertices *) vertices;
}

static unsigned short quadTriangles[6] = {0, 1, 2, 2, 3, 0};

static void initRegionAttachment(RegionAttachment* attachment, AtlasRegion* region) {
    if(!attachment || !region) {
        return;
    }
    attachment->setUVs(region->u, region->v, region->u2, region->v2, region->rotate);
    attachment->setRegionOffsetX(region->offsetX);
    attachment->setRegionOffsetY(region->offsetY);
    attachment->setRegionWidth(region->width);
    attachment->setRegionHeight(region->height);
    attachment->setRegionOriginalWidth(region->originalWidth);
    attachment->setRegionOriginalHeight(region->originalHeight);
    attachment->setWidth(region->width);
    attachment->setHeight(region->height);
}

static void initMeshAttachment(MeshAttachment* attachment, AtlasRegion* region) {
    if(!attachment || !region) {
        return;
    }
    attachment->setRegionU(region->u);
    attachment->setRegionV(region->v);
    attachment->setRegionU2(region->u2);
    attachment->setRegionV2(region->v2);
    attachment->setRegionRotate(region->rotate);
    attachment->setRegionDegrees(region->degrees);
    attachment->setRegionOffsetX(region->offsetX);
    attachment->setRegionOffsetY(region->offsetY);
    
    attachment->setRegionWidth(region->width);
    attachment->setRegionHeight(region->height);
    attachment->setRegionOriginalWidth(region->originalWidth);
    attachment->setRegionOriginalHeight(region->originalHeight);
    attachment->setWidth(region->width);
    attachment->setHeight(region->height);
}


static void setRegionAttachmentVertices(RegionAttachment* attachment, AtlasRegion* region) {
    if(!attachment || !region) {
        return;
    }
    
    auto attachmentVertices = new AttachmentVertices((Texture2D*)region->page->getRendererObject(), 4, quadTriangles, 6);
    V3F_C4B_T2F* vertices = attachmentVertices->_triangles->verts;
    for (int i = 0, ii = 0; i < 4; ++i, ii += 2) {
        vertices[i].texCoords.u = attachment->getUVs()[ii];
        vertices[i].texCoords.v = attachment->getUVs()[ii + 1];
    }
    attachment->setRendererObject(attachmentVertices, deleteAttachmentVertices);
}

static void setMeshAttachmentVertices(MeshAttachment* attachment, AtlasRegion* region) {
    if(!attachment || !region) {
        return;
    }
    AttachmentVertices* attachmentVertices = new AttachmentVertices((Texture2D*)region->page->getRendererObject(),
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

RTTI_IMPL(Cocos2dMultiAtlasAttachmentLoader, AttachmentLoader)

Cocos2dMultiAtlasAttachmentLoader::Cocos2dMultiAtlasAttachmentLoader(const String& defaultAttachmentName) : AttachmentLoader(), _defaultAttachmentName(defaultAttachmentName) {
    
}
Cocos2dMultiAtlasAttachmentLoader::~Cocos2dMultiAtlasAttachmentLoader() {
    
}
void Cocos2dMultiAtlasAttachmentLoader::addAtlas(Atlas *atlas) {
    _atlasList.add(atlas);
}
void Cocos2dMultiAtlasAttachmentLoader::removeAtlas(Atlas *atlas) {
    int idx = _atlasList.indexOf(atlas);
    if(idx >= 0) {
        _atlasList.removeAt(idx);
    }
}
AtlasRegion* Cocos2dMultiAtlasAttachmentLoader::findRegion(const String &name) {
    AtlasRegion *region = NULL;
    for(int i = _atlasList.size() - 1; i >= 0; --i) {
        region = _atlasList[i]->findRegion(name);
        if(region) {
            break;
        }
    }
    if(!region && _atlasList.size() > 0) {
        region = _atlasList[0]->findRegion(_defaultAttachmentName);
    }
    return region;
}
void Cocos2dMultiAtlasAttachmentLoader::refreshAttachment(Attachment* attachment) {
    auto region = findRegion(attachment->getName());
    if(!region) {
        return;
    }
    if (attachment->getRTTI().isExactly(RegionAttachment::rtti)) {
        auto regionAttachment = (RegionAttachment*)attachment;
        initRegionAttachment(regionAttachment, region);
        //regionAttachment->updateOffset();
        setRegionAttachmentVertices(regionAttachment, region);
    } else if (attachment->getRTTI().isExactly(MeshAttachment::rtti)) {
        auto meshAttachment = (MeshAttachment*)attachment;
        initMeshAttachment(meshAttachment, region);
        meshAttachment->updateUVs();
        setMeshAttachmentVertices(meshAttachment, region);
    }
}

RegionAttachment *Cocos2dMultiAtlasAttachmentLoader::newRegionAttachment(Skin &skin, const String &name, const String &path) {
    SP_UNUSED(skin);
    
    AtlasRegion *region = findRegion(path);
    if (!region) return NULL;

    RegionAttachment *attachment = new(__FILE__, __LINE__) RegionAttachment(name);
    attachment->setRendererObject(region);
    initRegionAttachment(attachment, region);
    return attachment;
}

MeshAttachment *Cocos2dMultiAtlasAttachmentLoader::newMeshAttachment(Skin &skin, const String &name, const String &path) {
    SP_UNUSED(skin);
    
    AtlasRegion *region = findRegion(path);
    if (!region) return NULL;

    MeshAttachment *attachment = new(__FILE__, __LINE__) MeshAttachment(name);
    attachment->setRendererObject(region);
    initMeshAttachment(attachment, region);
    return attachment;
}

BoundingBoxAttachment *Cocos2dMultiAtlasAttachmentLoader::newBoundingBoxAttachment(Skin &skin, const String &name) {
    SP_UNUSED(skin);
    return new(__FILE__, __LINE__) BoundingBoxAttachment(name);
}

PathAttachment *Cocos2dMultiAtlasAttachmentLoader::newPathAttachment(Skin &skin, const String &name) {
    SP_UNUSED(skin);
    return new(__FILE__, __LINE__) PathAttachment(name);
}

PointAttachment *Cocos2dMultiAtlasAttachmentLoader::newPointAttachment(Skin &skin, const String &name) {
    SP_UNUSED(skin);
    return new(__FILE__, __LINE__) PointAttachment(name);
}

ClippingAttachment *Cocos2dMultiAtlasAttachmentLoader::newClippingAttachment(Skin &skin, const String &name) {
    SP_UNUSED(skin);
    return new(__FILE__, __LINE__) ClippingAttachment(name);
}

void Cocos2dMultiAtlasAttachmentLoader::configureAttachment(Attachment* attachment) {
    if (attachment->getRTTI().isExactly(RegionAttachment::rtti)) {
        auto regionAttachment = (RegionAttachment*)attachment;
        auto region = (AtlasRegion*)regionAttachment->getRendererObject();
        if(!region) {
            return;
        }
        setRegionAttachmentVertices(regionAttachment, region);
    } else if (attachment->getRTTI().isExactly(MeshAttachment::rtti)) {
        auto meshAttachment = (MeshAttachment*)attachment;
        auto region = (AtlasRegion*)meshAttachment->getRendererObject();
        if(!region) {
            return;
        }
        setMeshAttachmentVertices(meshAttachment, region);
    }
}
