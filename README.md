# Pal-World_DirectX11_Team
 
DirectX11 팰월드 모작 팀 프로젝트

역할 : 이펙트, 건축시스템

[ 작업 내용 ]

- ToolEffect 프로젝트 전체

- Engine
   - Component
      - EffectComponent
      - ParticleComponent
      - EffectMeshComponent
      - InstanceEffectMeshComponent
      - TrailComponent
 
   - Buffer
      - Instancing
      - InstanceEffectMeshBuffer
      - TrailBuffer

- Client
   - Manager
      - BuildManager
      - EffectManager
 
   - Architecture
      - Architecture
      - ArchitecturePrefab
      - Housing
      - HousingPrefab
      - HousingLump
      - BerryFram
      - BlastFurnace
      - Chest
      - Door
      - FeedingBox
      - HumanBed
      - IronChest
      - KitchenMedieval
      - LumberYard
      - PalBed
      - PalBox
      - Ranch
      - RepairBench
      - SphereTable
      - StonePit
      - WheatFarm
      - WorkBench
  
   - Effect
      - EffectBase
      - ParticleObject
      - EffectMeshObject
      - InstanceEffectMeshObject
      - EffectGroup
      - ProjectileEffectGroup
      - PalSummonParticle
      - PalDeadParticle
      - PalRetrieveTrail
   
   - ShaderFiles
      - FX_Effect_PT2.hlsl
      - FX_EffectMesh_PNT2T.hlsl
      - FX_Instancing_PRULPC.hlsl
      - FX_MeshInstancing_PNT2TRULPP.hlsl
      - FX_Trail_PT2.hlsl
