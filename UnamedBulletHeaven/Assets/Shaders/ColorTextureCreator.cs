using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteAlways]
public class ColorTextureCreator : MonoBehaviour
{
    public bool apply;
    public Material redM;
    public Material blueM;
    public List<Color> reds;
    public List<Color> blues;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if(apply)
        {
            UpdateTextures();
            apply = false;
        }
    }

    void UpdateTextures()
    {
        var redT = new Texture2D(reds.Count, 1, TextureFormat.ARGB32, false);
        redT.filterMode = FilterMode.Point;

        // set the pixel values
        for (int i = 0; i < reds.Count; i++)
        {
            redT.SetPixel(i, 0, reds[i]);
        }
        // Apply all SetPixel calls
        redT.Apply();

        // connect texture to material of GameObject this script is attached to
        redM.SetTexture("_ColorTex",redT);



        var blueT = new Texture2D(blues.Count, 1, TextureFormat.ARGB32, false);
        blueT.filterMode = FilterMode.Point;

        // set the pixel values
        for (int i = 0; i < blues.Count; i++)
        {
            blueT.SetPixel(i, 0, blues[i]);
        }
        // Apply all SetPixel calls
        blueT.Apply();

        // connect texture to material of GameObject this script is attached to
        blueM.SetTexture("_ColorTex", blueT);
    }
}
