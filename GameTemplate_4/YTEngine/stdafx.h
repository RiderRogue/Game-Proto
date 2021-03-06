// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once
/*
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
*/


#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <math.h>
#include <vector>
#include <list>
#include <D3Dcompiler.h>
#include <Windows.h>
#include <d3d11.h>
#include <map>
#include <functional>
#include <algorithm>

//サウンド
#include <xaudio2.h>
#include <x3daudio.h>
#include <xaudio2fx.h>

#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"

//Effekseerのヘッダーファイルをインクルード。
#include "EffekseerRuntime130\src\Effekseer\Effekseer.h"
#include "EffekseerRuntime130/src/EffekseerRendererDX11/EffekseerRendererDX11.h"

//DirectXTKのインクルードファイル。
#include "DirectXTK/Inc/Model.h"
#include "DirectXTK/Inc/Effects.h"	
#include "DirectXTK/Inc/CommonStates.h"
#include "DirectXTK/Inc/SpriteFont.h"
#include "DirectXTK/Inc/DDSTextureLoader.h"

#include "physics/Physics.h"
#include "HID/Pad.h"

#include "math/Vector.h"
#include "math/Matrix.h"
#include "graphics/GraphicsEngine.h"

#include "graphics/SkinModel.h"
#include "graphics/Skeleton.h"

#include "policy/Noncopyable.h"
#include "policy/ScopedResource.h"

#include "util/NameKey.h"
#include "util/LocData.h"

#include "GameObject/GameObjectManager.h"

#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "graphics/Camera.h"
#include "timer/GameTime.h"

#include "YTEngine.h"

static const float FRAME_BUFFER_W = 1280.0f;				//フレームバッファの幅。
static const float FRAME_BUFFER_H = 720.0f;				//フレームバッファの高さ。

static const int MAX_BONE = 512;	//!<ボーンの最大数。


// TODO: プログラムに必要な追加ヘッダーをここで参照してください
