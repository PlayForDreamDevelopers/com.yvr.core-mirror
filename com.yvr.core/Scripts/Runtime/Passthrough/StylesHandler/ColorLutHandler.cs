using System;
using UnityEngine;

namespace YVR.Core
{
    internal class ColorLutHandler : IStyleHandler
    {
        protected bool m_CurrentFlipLutY;
        protected Texture2D m_CurrentColorLutSourceTexture;
        public PassthroughColorLut Lut { get; set; }
        public float weight { get; set; }

        public bool isValid { get; protected set; }

        public virtual void ApplyStyleSettings(ref PassthroughStyle style)
        {
            if (Lut == null || Lut.m_ColorLutHandle == 0)
            {
                isValid = false;
                return;
            }
            style.lutSource = Lut.m_ColorLutHandle;
            style.lutWeight = weight;
        }

        public virtual void Update(LutSettings settings)
        {
            Update(
                GetColorLutForTexture(settings.colorLutSourceTexture, Lut, ref m_CurrentColorLutSourceTexture,
                    settings.flipLutY),
                settings.lutWeight);
        }

        protected PassthroughColorLut GetColorLutForTexture(Texture2D newTexture, PassthroughColorLut lut,
            ref Texture2D lastTexture, bool flipY)
        {
            if (newTexture == null)
            {
                return lut;
            }

            if (lastTexture != newTexture || m_CurrentFlipLutY != flipY)
            {
                lut?.Dispose();
                lut = null;
                lastTexture = newTexture;
                m_CurrentFlipLutY = flipY;
                var colorLut = new PassthroughColorLut(newTexture, m_CurrentFlipLutY);
                return colorLut;
            }

            return lut;
        }

        internal void Update(PassthroughColorLut lut, float weight)
        {
            if (lut == null)
            {
                isValid = false;
            }
            else
            {
                isValid = true;
                Lut = lut;
                this.weight = weight;
            }
        }

        public virtual void Clear()
        {
            Lut?.Dispose();
            Lut = null;
            m_CurrentColorLutSourceTexture = null;
        }
    }
}