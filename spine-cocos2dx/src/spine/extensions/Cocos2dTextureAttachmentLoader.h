//
//  Cocos2dTextureAttachmentLoader.h
//  spine-cocos2d-x
//
//  Created by tapiream on 2021/7/19.
//

#ifndef Cocos2dTextureAttachmentLoader_h
#define Cocos2dTextureAttachmentLoader_h

#include <spine/spine.h>

namespace spine {

class Cocos2dTextureAttachmentLoader: public AttachmentLoader {
public:
    RTTI_DECL
    
    Cocos2dTextureAttachmentLoader();
    virtual ~Cocos2dTextureAttachmentLoader();
    
    virtual RegionAttachment* newRegionAttachment(Skin& skin, const String& name, const String& path);
    virtual MeshAttachment* newMeshAttachment(Skin& skin, const String& name, const String& path);
    virtual BoundingBoxAttachment* newBoundingBoxAttachment(Skin& skin, const String& name);
    virtual PathAttachment* newPathAttachment(Skin& skin, const String& name);
    virtual PointAttachment* newPointAttachment(Skin& skin, const String& name);
    virtual ClippingAttachment* newClippingAttachment(Skin& skin, const String& name);
    virtual void configureAttachment(Attachment* attachment);
    
    virtual void refreshAttachment(Attachment* attachment, const String& path);
protected:
    
};

}

#endif /* Cocos2dTextureAttachmentLoader_h */
