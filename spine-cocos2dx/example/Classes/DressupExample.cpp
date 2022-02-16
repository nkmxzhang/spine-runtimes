/******************************************************************************
 * Spine Runtimes License Agreement
 * Last updated January 1, 2020. Replaces all prior versions.
 *
 * Copyright (c) 2013-2020, Esoteric Software LLC
 *
 * Integration of the Spine Runtimes into software or otherwise creating
 * derivative works of the Spine Runtimes is permitted under the terms and
 * conditions of Section 2 of the Spine Editor License Agreement:
 * http://esotericsoftware.com/spine-editor-license
 *
 * Otherwise, it is permitted to integrate the Spine Runtimes into software
 * or otherwise create derivative works of the Spine Runtimes (collectively,
 * "Products"), provided that each user of the Products must obtain their own
 * Spine Editor license and redistribution of the Products in any form must
 * include this license and copyright notice.
 *
 * THE SPINE RUNTIMES ARE PROVIDED BY ESOTERIC SOFTWARE LLC "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ESOTERIC SOFTWARE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES,
 * BUSINESS INTERRUPTION, OR LOSS OF USE, DATA, OR PROFITS) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THE SPINE RUNTIMES, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include "DressupExample.h"
#include "BatchingExample.h"
#include "AttachmentVertices.h"
#include "spine-cocos2dx.h"
#include <spine/extensions/Cocos2dSpineHelper.h>

#define DEFAULT_SPINE_ATTACHMENT_NAME "000000/dress_blank"

USING_NS_CC;
using namespace spine;

Scene* DressupExample::scene () {
	Scene *scene = Scene::create();
	scene->addChild(DressupExample::create());
	return scene;
}

DressupExample::~DressupExample() {
    CC_SAFE_DELETE(skin);
    CC_SAFE_DELETE(_attachmentLoader);
    for(const auto& [k, v] : _atlasMap) {
        delete v;
    }
    _atlasMap.clear();
}

bool DressupExample::init () {
    if (!LayerColor::initWithColor(Color4B(128, 128, 128, 255))) return false;
    
    touchCount = 0;
    dressup3();
    //dressup1();
	return true;
}

void DressupExample::dressup3() {
    skinList = {};
    _skinAtlasMap = {
        
    };
    std::map<std::string,std::string> skinAtlasMap = {
        {"hair/300301", "300301.atlas"},
        {"upper/100002", "100002.atlas"},
        {"lower/100003", "100003.atlas"},
        {"shoes/100005", "100005.atlas"}
    };
    std::map<std::string,std::string> slotAtlasMap = {
        //{"sp1_1__tfh", "attachments/slot_hat_400106.png"}
    };
    skeletonNode = Cocos2dSpineHelper::createMixMatchBinarySpine(
                                                                 "me.skel",
                                                                 "000000.atlas",
                                                                 1,
                                                                 skinAtlasMap,
                                                                 slotAtlasMap,
                                                                 DEFAULT_SPINE_ATTACHMENT_NAME
                                                                 );
    skeletonNode->setAnimation(0, "idle", true);
    skeletonNode->setPosition(Vec2(_contentSize.width / 2, _contentSize.height / 2 - 200));
    addChild(skeletonNode);
    skeletonNode->setScale(-1, 1);
    
    scheduleUpdate();
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this] (Touch* touch, cocos2d::Event* event) -> bool {
//        if(touchCount < skinList.size())
//        {
//            auto skinName = skinList[touchCount];
//            auto atlasName = _skinAtlasMap[skinName];
//            Cocos2dSpineHelper::refreshSkin(skeletonNode, skinName.c_str(), atlasName.c_str(), DEFAULT_SPINE_ATTACHMENT_NAME);
//            touchCount++;
//        }
//        else if(touchCount == skinList.size()) {
//            touchCount++;
//            auto skin = skeletonNode->getSkeleton()->getSkin();
//            auto slotIdx = skeletonNode->getSkeleton()->findSlotIndex("qq2");
//            if(slotIdx >= 0) {
//                skin->removeAttachment(slotIdx, "sp1_1__tfh");
//                skeletonNode->setToSetupPose();
//            }
//        }
//        else if(touchCount == skinList.size() + 1) {
//            touchCount++;
//            auto skin = skeletonNode->getSkeleton()->getSkin();
//            auto slot = skeletonNode->getSkeleton()->findSlot("sp1_1__tfh");
//            if(slot) {
//                auto attachment = slot->getAttachment();
//                if(attachment) {
//                    //skin->addSkin(<#Skin *other#>)
//                }
//                skeletonNode->setToSetupPose();
//            }
//        }
//        else {
//            //remove avatar skin
//            auto skin = new (__FILE__, __LINE__) Skin("mix-and-match");
//            skeletonNode->getSkeleton()->setSkin(skin);
//            skeletonNode->setToSetupPose();
//            touchCount = 0;
//        }
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void DressupExample::dressup2() {
    skinList = {"skin-base", "nose/short", "eyelids/girly", "eyes/violet", "hair/brown", "clothes/hoodie-orange", "legs/pants-jeans", "accessories/bag", "accessories/hat-red-yellow"};
    
    // Load the texture atlas. Note that the texture loader has to live
    // as long as the Atlas, as the Atlas destructor will call TextureLoader::unload.
    _atlasMap["mix-and-match.atlas"] = new (__FILE__, __LINE__) Atlas("mix-and-match.atlas", &textureLoader, true);
    CCASSERT(_atlasMap["mix-and-match.atlas"], "Error reading atlas file.");
    _atlasMap["1.atlas"] = new (__FILE__, __LINE__) Atlas("1.atlas", &textureLoader, true);

    // This attachment loader configures attachments with data needed for cocos2d-x rendering.
    // Do not dispose the attachment loader until the skeleton data is disposed!
    _attachmentLoader = new (__FILE__, __LINE__) Cocos2dMultiAtlasAttachmentLoader(DEFAULT_SPINE_ATTACHMENT_NAME);
    //_attachmentLoader->addAtlas(_atlasMap["mix-and-match.atlas"]);
    _attachmentLoader->addAtlas(_atlasMap["1.atlas"]);

    // Load the skeleton data.
    SkeletonBinary* binary = new (__FILE__, __LINE__) SkeletonBinary(_attachmentLoader);
    binary->setScale(0.6f); // Resizes skeleton data to 60% of the size it was in Spine.
    auto skeletonData = binary->readSkeletonDataFile("mix-and-match-pro.skel");
    CCASSERT(skeletonData, binary->getError().isEmpty() ? binary->getError().buffer() : "Error reading skeleton data file.");
    delete binary;
    
    skeletonNode = SkeletonAnimation::createWithData(skeletonData, false);
    skin = new (__FILE__, __LINE__) Skin("mix-and-match");
    skeletonNode->getSkeleton()->setSkin(skin);
    skeletonNode->setAnimation(0, "dance", true);
    skeletonNode->setPosition(Vec2(_contentSize.width / 2, _contentSize.height / 2 - 200));
    addChild(skeletonNode);
    
    scheduleUpdate();
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this] (Touch* touch, cocos2d::Event* event) -> bool {
        if(touchCount < skinList.size())
        {
            SkeletonData* skeletonData = skeletonNode->getSkeleton()->getData();
            auto skin = skeletonNode->getSkeleton()->getSkin();
            if(skin)
            {
                _attachmentLoader->addAtlas(_atlasMap["mix-and-match.atlas"]);
                auto partSkin = skeletonData->findSkin(skinList[touchCount].c_str());
                skin->addSkin(partSkin);
                skeletonNode->getSkeleton()->setSkin(skin);
                skeletonNode->setToSetupPose();
                
                //refresh all slots in skin
                partSkin->getAttachments();
                auto slots = skeletonNode->getSkeleton()->getSlots();
                auto entries = partSkin->getAttachments();
                while (entries.hasNext()) {
                    Skin::AttachmentMap::Entry &entry = entries.next();
                    //int slotIndex = entry._slotIndex;
                    //Slot *slot = slots[slotIndex];
                    _attachmentLoader->refreshAttachment(entry._attachment);
                    
//                    if (slot->getAttachment() == entry._attachment) {
//                        Attachment *attachment = getAttachment(slotIndex, entry._name);
//                        if (attachment) slot->setAttachment(attachment);
//                    }
                }
                
                
                
                
                
                _attachmentLoader->removeAtlas(_atlasMap["mix-and-match.atlas"]);
                touchCount++;
            }
        }
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void DressupExample::dressup1() {
    slotAttachments =
    {
        {"gun", "gun-freeze.png"},
        {"goggles", "goggles-tactical.png"}
    };
    skeletonNode = SkeletonAnimation::createWithJsonFile("spineboy-pro.json", "spineboy.atlas", 0.6);
    skeletonNode->setAnimation(0, "walk", true);

    skeletonNode->setPosition(Vec2(_contentSize.width / 2, _contentSize.height / 2 - 200));
    addChild(skeletonNode);

    scheduleUpdate();

    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this] (Touch* touch, cocos2d::Event* event) -> bool {
//        if(touchCount < skinList.size())
//        {
//            SkeletonData* skeletonData = skeletonNode->getSkeleton()->getData();
//            auto skin = skeletonNode->getSkeleton()->getSkin();
//            if(skin)
//            {
//                skin->addSkin(skeletonData->findSkin(skinList[touchCount].c_str()));
//                skeletonNode->getSkeleton()->setSkin(skin);
//                skeletonNode->setToSetupPose();
//                touchCount++;
//            }
//        }
        switch (touchCount) {
            case 0:
                equip("gun");
                break;
            case 1:
                equip("goggles");
                break;
            default:
                break;
        }
        touchCount++;
        
//        if (!skeletonNode->getDebugBonesEnabled())
//            skeletonNode->setDebugBonesEnabled(true);
//        else if (skeletonNode->getTimeScale() == 1)
//            skeletonNode->setTimeScale(0.3f);
//        else
//            Director::getInstance()->replaceScene(BatchingExample::scene());
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


void DressupExample::equip(const std::string &slotName)
{
    auto skeletonData = skeletonNode->getSkeleton()->getData();
//    int slotIndex = skeletonData->findSlotIndex(slotName.c_str());
//
//    auto templateSkin = skeletonData->findSkin("base");
//    auto templateAttachment = templateSkin->getAttachment(slotIndex, slotName.c_str());
    //auto attachment = templateAttachment->getre(asset.sprite, sourceMaterial, premultiplyAlpha: this.applyPMA);
    // Note: Each call to `GetRemappedClone()` with parameter `premultiplyAlpha` set to `true` creates
    // a cached Texture copy which can be cleared by calling AtlasUtilities.ClearCache() as shown in the method below.
    //templateSkin->setattachment
    
    //equipsSkin.SetAttachment(slotIndex, attachmentName, attachment);
    //skeletonAnimation.Skeleton.SetSkin(equipsSkin);
    
    Cocos2dTextureAttachmentLoader attachmentLoader;
    auto templateSkin = skeletonData->findSkin("base");
    auto slot = skeletonNode->getSkeleton()->findSlot(slotName.c_str());
    
//    auto attachment = attachmentLoader.newRegionAttachment(*templateSkin, slotName.c_str(), slotAttachments[slotName].c_str());
//    auto originalAttachment = (RegionAttachment *)slot->getAttachment();
//    if(originalAttachment)
//    {
//        attachment->setWidth(attachment->getRegionOriginalWidth() * originalAttachment->getWidth() / originalAttachment->getRegionOriginalWidth());
//        attachment->setHeight(attachment->getRegionOriginalHeight() * originalAttachment->getHeight() / originalAttachment->getRegionOriginalHeight());
//        attachment->setRotation(originalAttachment->getRotation());
//    }
//    attachment->setScaleX(0.6);
//    attachment->setScaleY(0.6);
    
    
    auto attachment = slot->getAttachment();
    if(attachment)
    {
        attachmentLoader.refreshAttachment(attachment, slotAttachments[slotName].c_str());
    }
    
    
    
//    auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(slotAttachments[slotName]);
//
//    auto attachment = (RegionAttachment *)slot->getAttachment();
//    auto attachmentVertices = new AttachmentVertices(texture, 4, quadTriangles, 6);
//    V3F_C4B_T2F* vertices = attachmentVertices->_triangles->verts;
//    for (int i = 0, ii = 0; i < 4; ++i, ii += 2) {
//        vertices[i].texCoords.u = attachment->getUVs()[ii];
//        vertices[i].texCoords.v = attachment->getUVs()[ii + 1];
//    }
//    attachment->setRendererObject(attachmentVertices, deleteAttachmentVertices);
    
    
    
//    float wide = texture->getPixelsWide();
//    float high = texture->getPixelsHigh();
//    attachment->setUVs(0, 0, 1, 1, false);
//    attachment->setRegionWidth(wide);
//    attachment->setRegionHeight(high);
//    attachment->setRegionOriginalWidth(wide);
//    attachment->setRegionOriginalHeight(high);
//    attachment->setWidth(wide);
//    attachment->setHeight(high);
//    attachment->setScaleX(0.6f);
//    attachment->setScaleY(0.6f);

//    auto *attachV = (AttachmentVertices *)attachment->getRendererObject();
//    if (attachV->_texture == texture)
//        return;
//    CC_SAFE_RELEASE(attachV->_texture);
//    attachV->_texture = texture;
//    CC_SAFE_RETAIN(texture);
//
//    auto *vertices = attachV->_triangles->verts;
//    for (int i = 0, ii = 0; i < 4; ++i, ii += 2)
//    {
//        vertices[i].texCoords.u = attachment->getUVs()[ii];
//        vertices[i].texCoords.v = attachment->getUVs()[ii + 1];
//    }

    //attachment->updateOffset();
    //slot->setAttachment(attachment);
    //skeletonNode->setToSetupPose();
}
