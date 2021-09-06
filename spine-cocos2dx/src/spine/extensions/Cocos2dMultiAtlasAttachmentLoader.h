//
//  Cocos2dMultiAtlasAttachmentLoader.h
//  spine-cocos2d-x
//
//  Created by tapiream on 2021/7/19.
//

#ifndef Cocos2dMultiAtlasAttachmentLoader_h
#define Cocos2dMultiAtlasAttachmentLoader_h

#include <spine/spine.h>

namespace spine {

class Cocos2dMultiAtlasAttachmentLoader: public AttachmentLoader {
public:
    RTTI_DECL
    
    Cocos2dMultiAtlasAttachmentLoader(const String& defaultAttachmentName);
    virtual ~Cocos2dMultiAtlasAttachmentLoader();
    
    virtual RegionAttachment* newRegionAttachment(Skin& skin, const String& name, const String& path);
    virtual MeshAttachment* newMeshAttachment(Skin& skin, const String& name, const String& path);
    virtual BoundingBoxAttachment* newBoundingBoxAttachment(Skin& skin, const String& name);
    virtual PathAttachment* newPathAttachment(Skin& skin, const String& name);
    virtual PointAttachment* newPointAttachment(Skin& skin, const String& name);
    virtual ClippingAttachment* newClippingAttachment(Skin& skin, const String& name);
    virtual void configureAttachment(Attachment* attachment);
    
    virtual AtlasRegion* findRegion(const String& name);
    virtual void refreshAttachment(Attachment* attachment);
    
    void addAtlas(Atlas* atlas);
    void removeAtlas(Atlas* atlas);
private:
    Vector<Atlas*> _atlasList;
    String _defaultAttachmentName;
};

}

#endif /* Cocos2dMultiAtlasAttachmentLoader_h */
