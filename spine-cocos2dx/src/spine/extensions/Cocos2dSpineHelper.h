//
//  Cocos2dSpineHelper.h
//  spine-cocos2d-x
//
//  Created by tapiream on 2021/7/19.
//

#ifndef Cocos2dSpineHelper_h
#define Cocos2dSpineHelper_h

#include <spine/spine.h>
#include <spine/SkeletonAnimation.h>
#include <map>
#include <string>

namespace spine {


class Cocos2dSpineHelper {
public:
    /**
     * Create mix and match SkeletonRenderer with binary skel data. refresh the skins and slots with atlas
     *
     * @param skelPath The binary skel file path of the SkeletonRenderer
     * @param atlasPath The basic atlas file path of the SkeletonRenderer
     * @param scale The initialize scale of the SkeletonRenderer
     * @param skinAtlasMap The skins and atlases which need to be updated. NOTE: only mix skin while atlas is empty
     * @param slotAtlasMap The slots and atlases which need to be updated. NOTE: 1) atlas is a .atlas or .png. 2) exclude all attachments of slot while atlas is empty
     * @param defaultRegionName The default region name of attachment while loading SkeletonRenderer
     *
     * @return SkeletonRenderer
     */
    static SkeletonAnimation* createMixMatchBinarySpine(const char *skelPath,
                                                        const char* atlasPath,
                                                        const float scale,
                                                        const std::map<std::string, std::string>& skinAtlasMap,
                                                        const std::map<std::string, std::string>& slotAtlasMap,
                                                        const char* defaultRegionName);
    /**
     * Create mix and match SkeletonRenderer with JSON data. refresh the skins and slots with atlas
     *
     * @param jsonPath The JSON file path of the SkeletonRenderer
     * @param atlasPath The basic atlas file path of the SkeletonRenderer
     * @param scale The initialize scale of the SkeletonRenderer
     * @param skinAtlasMap The skins and atlases which need to be updated. NOTE: only mix skin while atlas is empty
     * @param slotAtlasMap The slots and atlases which need to be updated. NOTE: 1) atlas is a .atlas or .png. 2) exclude all attachments of slot while atlas is empty
     * @param defaultRegionName The default region name of attachment while loading SkeletonRenderer
     *
     * @return SkeletonRenderer
     */
    static SkeletonAnimation* createMixMatchJsonSpine(const char *jsonPath,
                                                      const char* atlasPath,
                                                      const float scale,
                                                      const std::map<std::string, std::string>& skinAtlasMap,
                                                      const std::map<std::string, std::string>& slotAtlasMap,
                                                      const char* defaultRegionName);
    /**
     * Refresh the skins and slots of  the SkeletonRenderer with atlas
     *
     * @param skeletonNode The SkeletonRenderer
     * @param scale The initialize scale of the SkeletonRenderer
     * @param skinAtlasMap The skins and atlases which need to be updated. NOTE: only mix skin while atlas is empty
     * @param slotAtlasMap The slots and atlases which need to be updated. NOTE: 1) atlas is a .atlas or .png. 2) exclude all attachments of slot while atlas is empty
     * @param defaultRegionName The default region name of attachment while loading SkeletonRenderer
     */
    static void refreshSpine(SkeletonAnimation* skeletonNode,
                             const float scale,
                             const std::map<std::string, std::string>& skinAtlasMap,
                             const std::map<std::string, std::string>& slotAtlasMap,
                             const char* defaultRegionName);
};

}

#endif /* Cocos2dSpineHelper_h */
