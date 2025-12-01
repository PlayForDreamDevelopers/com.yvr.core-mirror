using System;
using UnityEngine;

namespace YVR.Core
{
    public class PassthroughLayer : MonoBehaviour
    {
        private bool styleDirty = true;

        private StylesHandler m_StylesHandler = new ();

        [SerializeField] private float m_LutWeight = 1f;

        [SerializeField] private bool m_FlipLutY = true;

        private LutSettings m_Settings = new LutSettings(null, 1, true);

        #region Editor

        [SerializeField] internal PassthroughColorMapType m_ColorMapType = PassthroughColorMapType.None;

        /// <summary>
        /// Editor attribute to get or set the selection in the inspector.
        /// Using this selection will update the `colorMapType` and `colorMapData` if needed.
        /// </summary>
        public PassthroughColorMapType colorMapType
        {
            get { return m_ColorMapType; }
            set
            {
                if (value != m_ColorMapType)
                {
                    m_ColorMapType = value;
                    m_StylesHandler.SetStyleHandler(colorMapType);
                    if (value == PassthroughColorMapType.None)
                    {
                        styleDirty = true;
                    }
                    else
                    {
                        UpdateColorMapFromControls(true);
                    }
                }
            }
        }

        [SerializeField] internal Texture2D m_ColorLutSourceTexture;


        public float lutWeight
        {
            get { return m_LutWeight;}
            set
            {
                m_LutWeight = ClampWeight(value);
                UpdateColorMapFromControls(true);
            }
        }

        public bool flipLutY
        {
            get { return m_FlipLutY;}
            set
            {
                m_FlipLutY = value;
                UpdateColorMapFromControls(true);
            }
        }

        /// <summary>
        /// This method is required for internal use only.
        /// </summary>
        public void SetStyleDirty()
        {
            styleDirty = true;
        }

    #endregion

        void Start()
        {
            m_StylesHandler.SetStyleHandler(colorMapType);

            if (HasControlsBasedColorMap())
            {
                // Compute initial color map from controls
                UpdateColorMapFromControls(true);
            }

            // Flag style to be re-applied in LateUpdate()
            styleDirty = true;
        }

        void LateUpdate()
        {
            // Update passthrough color map with gradient if it was changed in the inspector.
            UpdateColorMapFromControls();

            // Passthrough style updates are buffered and committed to the API atomically here.
            if (styleDirty)
            {
                if (m_StylesHandler.currentStyleHandler.isValid)
                {
                    var passthroughStyle =  CreatePluginStyleObject();
                    YVRPlugin.Instance.SetPassthroughStyle(passthroughStyle);
                }

                styleDirty = false;
            }
        }

        private PassthroughStyle CreatePluginStyleObject()
        {
            PassthroughStyle style = default;
            style.textureColorMapType = colorMapType;

            m_StylesHandler.currentStyleHandler.ApplyStyleSettings(ref style);

            return style;
        }

        private bool HasControlsBasedColorMap()
        {
            return colorMapType != PassthroughColorMapType.None;
        }

        private void UpdateColorMapFromControls(bool forceUpdate = false)
        {
            bool parametersChanged = m_Settings.colorLutSourceTexture != m_ColorLutSourceTexture
                                     || m_Settings.lutWeight != m_LutWeight
                                     || m_Settings.flipLutY != m_FlipLutY;

            if (!(HasControlsBasedColorMap() && parametersChanged || forceUpdate))
                return;

            m_Settings.lutWeight = m_LutWeight;
            m_Settings.flipLutY = m_FlipLutY;
            m_Settings.colorLutSourceTexture = m_ColorLutSourceTexture;

            if (Application.isPlaying)
            {
                m_StylesHandler.currentStyleHandler.Update(m_Settings);
                styleDirty = true;
            }
        }

        public void SetColorLut(PassthroughColorLut lut)
        {
            if (lut != null && lut.IsValid)
            {
                colorMapType = PassthroughColorMapType.ColorLut;
                m_StylesHandler.SetColorLutHandler(lut, m_Settings.lutWeight);
                styleDirty = true;
            }
            else
            {
                Debug.LogError("Trying to set an invalid Color LUT for Passthrough");
            }
        }

        private static float ClampWeight(float weight)
        {
            if (weight < 0 || weight > 1)
            {
                Debug.LogWarning("Color lut weight should be between in [0, 1] range. Setting it to closest value.");
                weight = Mathf.Clamp01(weight);
            }

            return weight;
        }
    }
}