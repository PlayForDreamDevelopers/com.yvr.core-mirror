using System.Runtime.InteropServices;
using UnityEngine;

namespace YVR.Core
{
    internal class StylesHandler : MonoBehaviour
    {
        private NoneStyleHandler m_NoneHandler;
        private ColorLutHandler m_LutHandler;

        public IStyleHandler currentStyleHandler;

        public StylesHandler()
        {
            m_NoneHandler = new NoneStyleHandler();
            m_LutHandler = new ColorLutHandler();
        }

        public void SetStyleHandler(PassthroughColorMapType type)
        {
            var nextStyleHandler = GetStyleHandler(type);

            if (currentStyleHandler != null)
            {
                currentStyleHandler.Clear();
            }

            currentStyleHandler = nextStyleHandler;
        }

        private IStyleHandler GetStyleHandler(PassthroughColorMapType type)
        {
            switch (type)
            {
                case PassthroughColorMapType.None:
                    return m_NoneHandler;
                case PassthroughColorMapType.ColorLut:
                    return m_LutHandler;
                default:
                    throw new System.ArgumentException($"Unrecognized color map type {type}.");
            }
        }
        public void SetColorLutHandler(PassthroughColorLut lut, float weight)
        {
            SetStyleHandler(PassthroughColorMapType.ColorLut);
            m_LutHandler.Update(lut, weight);
        }
    }
}