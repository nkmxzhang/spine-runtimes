//
//  Cocos2dSpineHelper.cpp
//  spine-cocos2d-x
//
//  Created by tapiream on 2021/7/19.
//

#include "Cocos2dSpineHelper.h"
#include "Cocos2dMultiAtlasAttachmentLoader.h"
#include "Cocos2dTextureAttachmentLoader.h"
#include <spine/spine-cocos2dx.h>

#define MIX_MATCH_SKIN_NAME "mix-match"

USING_NS_CC;
using namespace spine;

static void _refreshSkins(Cocos2dMultiAtlasAttachmentLoader* attachmentLoader,
                          Cocos2dTextureLoader* textureLoader,
                          SkeletonAnimation *skeletonNode,
                          const float scale,
                          const std::map<std::string, std::string> &skinAtlasMap,
                          const std::map<std::string, std::string>& slotAtlasMap) {
    auto skin = new (__FILE__, __LINE__) Skin(MIX_MATCH_SKIN_NAME);
    skeletonNode->getSkeleton()->setSkin(skin);
    std::map<std::string, Atlas*> atlasMap;
    // Load the skeleton data.
    SkeletonData* skeletonData = skeletonNode->getSkeleton()->getData();
    for(const auto& kv : skinAtlasMap) {
        auto partSkin = skeletonData->findSkin(kv.first.c_str());
        if(!partSkin) {
            continue;
        }
        skin->addSkin(partSkin);
        if(kv.second.size() == 0) {
            //skin need NOT be refreshed
            continue;
        }
        auto atlas = atlasMap[kv.second];
        if(!atlas) {
            atlasMap[kv.second] = atlas = new (__FILE__, __LINE__) Atlas(kv.second.c_str(), textureLoader, true);
        }
        if(!atlas) {
            //skin need NOT be refreshed
            continue;
        }
        
        attachmentLoader->addAtlas(atlas);
        //refresh all slots in skin
        auto entries = partSkin->getAttachments();
        while (entries.hasNext()) {
            Skin::AttachmentMap::Entry &entry = entries.next();
            attachmentLoader->refreshAttachment(entry._attachment);
        }
        attachmentLoader->removeAtlas(atlas);
    }
    //load slot & attachment
    std::string suffix(".atlas");
    Cocos2dTextureAttachmentLoader textureAttachmentLoader;
    for(const auto& kv : slotAtlasMap) {
        auto slotIdx = skeletonNode->getSkeleton()->findSlotIndex(kv.first.c_str());
        if(slotIdx < 0) {
            //invalid slot
            continue;
        }
        if(kv.second.size() > 0) {
            auto attachment = skin->getAttachment(slotIdx, kv.first.c_str());
            //refresh the attachments of the slot
            if(kv.second.size() >= suffix.size() && 0 == kv.second.compare (kv.second.size() - suffix.size(), suffix.size(), suffix)) {
                //atlas
                auto atlas = atlasMap[kv.second];
                if(!atlas) {
                    atlasMap[kv.second] = atlas = new (__FILE__, __LINE__) Atlas(kv.second.c_str(), textureLoader, true);
                }
                if(!atlas) {
                    //slot need NOT be refreshed
                    continue;
                }
                attachmentLoader->addAtlas(atlas);
                //refresh or create attachment for slot
                if(attachment) {
                    attachmentLoader->refreshAttachment(attachment);
                } else {
                    auto regionAttachment = attachmentLoader->newRegionAttachment(*skin, kv.first.c_str(), kv.first.c_str());
                    if(regionAttachment) {
                        regionAttachment->setScaleX(scale);
                        regionAttachment->setScaleY(scale);
                        regionAttachment->updateOffset();
                        attachment = regionAttachment;
                        attachmentLoader->configureAttachment(attachment);
                        skin->setAttachment(slotIdx, kv.first.c_str(), attachment);
                    }
                }
                attachmentLoader->removeAtlas(atlas);
            } else {
                //texture
                if(attachment) {
                    textureAttachmentLoader.refreshAttachment(attachment, kv.second.c_str());
                } else {
                    auto regionAttachment = textureAttachmentLoader.newRegionAttachment(*skin, kv.first.c_str(), kv.second.c_str());
                    if(regionAttachment) {
                        regionAttachment->setScaleX(scale);
                        regionAttachment->setScaleY(scale);
                        regionAttachment->updateOffset();
                        attachment = regionAttachment;
                        textureAttachmentLoader.configureAttachment(attachment);
                        skin->setAttachment(slotIdx, kv.first.c_str(), attachment);
                    }
                }
            }
        } else {
            //exclusive all attachments of the slot
            skin->removeAttachment(slotIdx, kv.first.c_str());
        }
    }
    
    //release atlas map
    for(const auto& kv : atlasMap) {
        if(kv.second) {
            delete kv.second;
        }
    }
    atlasMap.clear();
    skeletonNode->setToSetupPose();
}

static SkeletonAnimation* _createMixMatchSpine(const char *spinePath,
                                               const char *atlasPath,
                                               const float scale,
                                               const std::map<std::string, std::string>& skinAtlasMap,
                                               const std::map<std::string, std::string>& slotAtlasMap,
                                               const char *defaultRegionName,
                                               bool isBinary) {
    spine::Cocos2dTextureLoader textureLoader;
    auto atlas = new (__FILE__, __LINE__) Atlas(atlasPath, &textureLoader, true);
    CCASSERT(atlas, "Error reading atlas file.");

    // This attachment loader configures attachments with data needed for cocos2d-x rendering.
    // Do not dispose the attachment loader until the skeleton data is disposed!
    auto attachmentLoader = new (__FILE__, __LINE__) Cocos2dMultiAtlasAttachmentLoader(defaultRegionName);
    attachmentLoader->addAtlas(atlas);

    SkeletonData* skeletonData = NULL;
    // Load the skeleton data.
    if(isBinary) {
        SkeletonBinary* binary = new (__FILE__, __LINE__) SkeletonBinary(attachmentLoader);
        binary->setScale(scale);
        skeletonData = binary->readSkeletonDataFile(spinePath);
        CCASSERT(skeletonData, binary->getError().isEmpty() ? binary->getError().buffer() : "Error reading skeleton data file.");
        delete binary;
    } else {
        SkeletonJson* json = new (__FILE__, __LINE__) SkeletonJson(attachmentLoader);
        json->setScale(scale);
        skeletonData = json->readSkeletonDataFile(spinePath);
        CCASSERT(skeletonData,json->getError().isEmpty() ? json->getError().buffer() : "Error reading skeleton data file.");
        delete json;
    }
    auto skeletonNode = SkeletonAnimation::createWithData(skeletonData, false);
    _refreshSkins(attachmentLoader, &textureLoader, skeletonNode, scale, skinAtlasMap, slotAtlasMap);
    
    attachmentLoader->removeAtlas(atlas);
    delete atlas;
    delete attachmentLoader;
    return skeletonNode;
}

SkeletonAnimation* Cocos2dSpineHelper::createMixMatchBinarySpine(const char *skelPath,
                                                                 const char *atlasPath,
                                                                 const float scale,
                                                                 const std::map<std::string, std::string>& skinAtlasMap,
                                                                 const std::map<std::string, std::string>& slotAtlasMap,
                                                                 const char *defaultRegionName) {
    return _createMixMatchSpine(skelPath, atlasPath, scale, skinAtlasMap, slotAtlasMap, defaultRegionName, true);
}

SkeletonAnimation* Cocos2dSpineHelper::createMixMatchJsonSpine(const char *jsonPath,
                                                               const char *atlasPath,
                                                               const float scale,
                                                               const std::map<std::string, std::string>& skinAtlasMap,
                                                               const std::map<std::string, std::string>& slotAtlasMap,
                                                               const char *defaultRegionName) {
    return _createMixMatchSpine(jsonPath, atlasPath, scale, skinAtlasMap, slotAtlasMap, defaultRegionName, false);
}

void Cocos2dSpineHelper::refreshSpine(SkeletonAnimation *skeletonNode,
                                      const float scale,
                                      const std::map<std::string, std::string> &skinAtlasMap,
                                      const std::map<std::string, std::string>& slotAtlasMap,
                                      const char *defaultRegionName) {
    spine::Cocos2dTextureLoader textureLoader;
    // This attachment loader configures attachments with data needed for cocos2d-x rendering.
    // Do not dispose the attachment loader until the skeleton data is disposed!
    auto attachmentLoader = new (__FILE__, __LINE__) Cocos2dMultiAtlasAttachmentLoader(defaultRegionName);
    _refreshSkins(attachmentLoader, &textureLoader, skeletonNode, scale, skinAtlasMap, slotAtlasMap);
    delete attachmentLoader;
}
