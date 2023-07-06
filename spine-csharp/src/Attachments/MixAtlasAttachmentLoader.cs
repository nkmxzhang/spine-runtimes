namespace Spine
{
    public class MixAtlasAttachmentLoader : AtlasAttachmentLoader
    {
        private string _defaultAttachmentName;
        private AtlasRegion _emptyRegion;

        public MixAtlasAttachmentLoader(Atlas[] atlasArray, string defaultAttachmentName) : base(atlasArray)
        {
            _defaultAttachmentName = defaultAttachmentName;
        }
    
        public override AtlasRegion FindRegion (string name) {
            for (int i = 0; i < atlasArray.Length; i++) {
                var region = atlasArray[i].FindRegion(name);
                if (region != null)
                    return region;
                if(_emptyRegion == null)
                    _emptyRegion = atlasArray[i].FindRegion(_defaultAttachmentName);
            }
            return _emptyRegion;
        }
    }
}

