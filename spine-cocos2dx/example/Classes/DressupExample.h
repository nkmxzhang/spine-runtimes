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

#ifndef _DRESSUPEXAMPLE_H_
#define _DRESSUPEXAMPLE_H_

#include "cocos2d.h"
#include <spine/spine-cocos2dx.h>
#include <spine/extensions/Cocos2dTextureAttachmentLoader.h>
#include <spine/extensions/Cocos2dMultiAtlasAttachmentLoader.h>
#include <string>
#include <vector>
#include <map>

class DressupExample : public cocos2d::LayerColor {
public:
	static cocos2d::Scene* scene ();

	CREATE_FUNC(DressupExample);

	virtual bool init ();
	
	virtual ~DressupExample();

private:
    void dressup1();
    void dressup2();
    void dressup3();
    void equip(const std::string& slotName);
    
	spine::SkeletonAnimation* skeletonNode;
	spine::Skin* skin;
    int touchCount;
    std::vector<std::string> skinList;
    std::map<std::string, std::string> slotAttachments;
    std::map<std::string, spine::Atlas*> _atlasMap;
    spine::Cocos2dMultiAtlasAttachmentLoader* _attachmentLoader;
    spine::Cocos2dTextureLoader textureLoader;
    
    std::map<std::string, std::string> _skinAtlasMap;
};

#endif // _DRESSUPEXAMPLE_H_
