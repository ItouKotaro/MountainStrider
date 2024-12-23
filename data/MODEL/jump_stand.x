xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 28;
 -15.43289;20.01227;-10.00000;,
 15.43289;20.01227;-10.00000;,
 15.43289;0.01227;-36.21120;,
 -15.43289;0.01227;-36.21120;,
 15.43289;20.01227;-10.00000;,
 15.43289;27.82159;0.00000;,
 15.43289;0.01227;0.00000;,
 15.43289;0.01227;-36.21120;,
 15.43289;20.01227;10.00000;,
 15.43289;0.01227;36.21120;,
 15.43289;20.01227;10.00000;,
 -15.43289;20.01227;10.00000;,
 -15.43289;0.01227;36.21120;,
 15.43289;0.01227;36.21120;,
 -15.43289;20.01227;10.00000;,
 -15.43289;27.82159;0.00000;,
 -15.43289;0.01227;0.00000;,
 -15.43289;0.01227;36.21120;,
 -15.43289;20.01227;-10.00000;,
 -15.43289;0.01227;-36.21120;,
 15.43289;27.82159;0.00000;,
 -15.43289;27.82159;0.00000;,
 -15.43289;27.82159;0.00000;,
 15.43289;27.82159;0.00000;,
 -15.43289;0.01227;0.00000;,
 15.43289;0.01227;0.00000;,
 -15.43289;0.01227;-36.21120;,
 15.43289;0.01227;-36.21120;;
 
 10;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;5,8,9,6;,
 4;10,11,12,13;,
 4;14,15,16,17;,
 4;15,18,19,16;,
 4;11,10,20,21;,
 4;22,23,1,0;,
 4;24,25,9,17;,
 4;26,27,25,24;;
 
 MeshMaterialList {
  3;
  10;
  0,
  2,
  2,
  1,
  2,
  2,
  1,
  0,
  2,
  2;;
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\jump.png";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\jump.png";
   }
  }
  Material {
   0.298039;0.373333;0.442353;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  9;
  0.000000;0.791585;-0.611059;,
  0.000000;0.794999;-0.606610;,
  1.000000;0.000000;0.000000;,
  0.000000;0.791585;0.611059;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.794999;0.606610;,
  0.000000;0.788146;0.615488;,
  0.000000;0.788146;-0.615488;,
  0.000000;-1.000000;-0.000000;;
  10;
  4;0,0,1,1;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;3,3,5,5;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;3,3,6,6;,
  4;7,7,0,0;,
  4;8,8,8,8;,
  4;8,8,8,8;;
 }
 MeshTextureCoords {
  28;
  0.002500;0.272245;,
  0.997526;0.272271;,
  0.997485;1.014424;,
  0.002460;1.014398;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.002078;0.274492;,
  0.997053;0.273829;,
  0.998022;1.016021;,
  0.003047;1.016684;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.001750;-0.011097;,
  0.996726;-0.011761;,
  0.002516;-0.013379;,
  0.997541;-0.013352;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}
