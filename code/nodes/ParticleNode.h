/*
  
  currently we need to set particle amount in:
  - this file
  - boidsim (as uniform)
  - sort (as uniform)
  
  grid cell count:
  - this file
  - boidsim
  - cellindex
  
  workgroup size
  - this file
  - boidsim
  - cellindex
  - offsetshader
  - sort

  
  Compile time
  TOTAL_GRID_CELL_COUNT
  WORKGROUP_SIZE
  
  
  Runtime
  PARTICLE_AMOUNT
  

 */

#pragma once

// needs to be hardcoded in shaders:
#define WORKGROUP_SIZE 32
#define TOTAL_GRID_CELL_COUNT 1024
/* #define PARTICLE_AMOUNT 1024 */
/* #define SORT_WORKGROUP_COUNT (PARTICLE_AMOUNT / (WORKGROUP_SIZE * 2)) */

struct Particle {
	vec4 pos;
	vec4 vel;
};

struct ParticleNode
{
	u32 bufferParticles;
	u32 bufferParticleIndices;
	u32 bufferCellIndices;
	u32 bufferCellOffset;

	i32 amount;

	Particle *particles;
	u32 *cellOffsets;
	u32 *indices;
};

struct ParticleNodeState
{
	Shader cellIndexShader;
	Shader sortShader;
	Shader offsetShader;
	Shader boidSimShader;

	ObjectContainer<ParticleNode> particleNodes;
};

global ParticleNodeState *_particleNodeState;

void SortDispatch(ParticleNode *particleNode, int n, int h, int algo){
	_particleNodeState->sortShader.Begin();
	_particleNodeState->sortShader.SetUniform("algo", algo);
	_particleNodeState->sortShader.SetUniform("h", h);

	GFXBufferBindBase(particleNode->bufferParticleIndices, 0);
	GFXBufferBindBase(particleNode->bufferCellIndices, 1);

	GFXCompute(particleNode->amount / (WORKGROUP_SIZE * 2), 1, 1);
	GFXMemBarrier(GFX_BARRIER_STORAGE);
	_particleNodeState->sortShader.End();
}

void LocalBms(ParticleNode *particleNode, int n, int h)
{
	SortDispatch(particleNode, n, h, 0);
}

void BigFlip(ParticleNode *particleNode, int n, int h)
{
	SortDispatch(particleNode, n, h, 2);
}

void LocalDisperse(ParticleNode *particleNode, int n, int h)
{
	SortDispatch(particleNode, n, h, 1);
}

void BigDisperse(ParticleNode *particleNode, int n, int h)
{
	SortDispatch(particleNode, n, h, 3);
}

void ParticleOp(Node *self)
{
	Mesh *output = GetMeshes()->Get(self->GetDataLast());
	if(!output) {
		DebugLog("ParticleNode: could not get mesh output");
		return;
	}

	ParticleNode *state = _particleNodeState->particleNodes.Get(self->extraHandle); 

	// Parameters
	int amount = self->params[0].Int();
	double avg = self->params[1].Double();
	double avoid = self->params[2].Double();
	double center = self->params[3].Double();
	double goal = self->params[4].Double();
	double maxRange = self->params[5].Double();
	double minDist = self->params[6].Double();
	double speed = self->params[7].Double();
	double limit = self->params[8].Double();


	if(!self->initialized || amount != state->amount) {
		self->initialized = true;

		state->amount = amount;
		_particleNodeState->sortShader.Begin();
		_particleNodeState->sortShader.SetUniform("n", amount);
		_particleNodeState->sortShader.End();
		
		// initialize buffer
		GFXBufferSetData(state->bufferParticles,
						 amount * sizeof(Particle),
						 GFX_DYNAMIC_READ,
						 NULL);

		GFXBufferSetData(state->bufferParticleIndices,
						 amount * sizeof(u32),
						 GFX_DYNAMIC_READ);
		state->indices = (u32*)GFXBufferMapRange(state->bufferParticleIndices,
												   amount * sizeof(u32));

		GFXBufferSetData(state->bufferCellIndices,
						 amount * sizeof(u32),
						 GFX_DYNAMIC_READ);
		_particleNodeState->sortShader.Begin();
		_particleNodeState->sortShader.SetUniform("n", amount);
		_particleNodeState->sortShader.End();

		GFXBufferSetData(state->bufferCellOffset,
						 TOTAL_GRID_CELL_COUNT * sizeof(u32),
						 GFX_DYNAMIC_READ);
		state->cellOffsets = (u32*)GFXBufferMapRange(state->bufferCellOffset,
												   TOTAL_GRID_CELL_COUNT * sizeof(u32));

		output->SetVertexAmount(amount);

		state->particles = (Particle*)GFXBufferMap(state->bufferParticles);
		for(i32 i = 0; i < amount; i++) {
			state->particles[i].pos.x = RandFloat() * 0.3f;
			state->particles[i].pos.y = RandFloat() * 0.3f;
			state->particles[i].pos.z = RandFloat() * 0.3f;

			state->indices[i] = i;

			/* state->particles[i].vel.x = RandFloat(); */
			/* state->particles[i].vel.y = RandFloat(); */
			/* state->particles[i].vel.z = RandFloat(); */
		}
		GFXBufferUnmap(state->bufferParticles);

	}

	_particleNodeState->boidSimShader.Begin();
	_particleNodeState->boidSimShader.SetUniform("config.avg", (float)avg);
	_particleNodeState->boidSimShader.SetUniform("config.avoid", (float)avoid);
	_particleNodeState->boidSimShader.SetUniform("config.center", (float)center);
	_particleNodeState->boidSimShader.SetUniform("config.goal", (float)goal);
	_particleNodeState->boidSimShader.SetUniform("config.maxRange", (float)maxRange);
	_particleNodeState->boidSimShader.SetUniform("config.minDist", (float)minDist);
	_particleNodeState->boidSimShader.SetUniform("config.speed", (float)speed);
	_particleNodeState->boidSimShader.SetUniform("config.limit", (float)limit);
	_particleNodeState->boidSimShader.End();

	// CELL INDEXING
	_particleNodeState->cellIndexShader.Begin();
	GFXBufferBindBase(state->bufferParticles, 0);
	GFXBufferBindBase(state->bufferParticleIndices, 1);
	GFXBufferBindBase(state->bufferCellIndices, 2);
	GFXCompute(amount / WORKGROUP_SIZE, 1, 1);
	GFXMemBarrier(GFX_BARRIER_STORAGE);
	_particleNodeState->cellIndexShader.End();
	GFXFinish();


	// MERGE SORT
	u32 h = WORKGROUP_SIZE * 2;
	LocalBms(state, amount, h);
	for ( ; h <= amount; h *= 2 ) {
		BigFlip(state, amount, h);
		for ( uint32_t hh = h / 2; hh > 1; hh /= 2 ) {
			if ( hh <= WORKGROUP_SIZE * 2 ) {
				LocalDisperse(state, amount, hh);
				break;
			}
			else {
				BigDisperse(state, amount, hh);
			}
		}
	}
	GFXFinish();

	// CREATE OFFSET BUFFER
	_particleNodeState->offsetShader.Begin();
	memset(state->cellOffsets, 0xFF, sizeof(u32) * TOTAL_GRID_CELL_COUNT);
	GFXBufferBindBase(state->bufferParticleIndices, 1);
	GFXBufferBindBase(state->bufferCellIndices, 2);
	GFXBufferBindBase(state->bufferCellOffset, 3);
	GFXCompute(amount / WORKGROUP_SIZE, 1, 1);
	GFXMemBarrier(GFX_BARRIER_STORAGE);
	_particleNodeState->offsetShader.End();
	GFXFinish();


	// BOID SIM
	_particleNodeState->boidSimShader.Begin();
	/* boidShader.setUniform1i("n", PARTICLE_AMOUNT); */
	vec4 goalPos = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	/* boidShader.setUniform1i("n", PARTICLE_AMOUNT); */
	_particleNodeState->boidSimShader.SetUniform("n", amount);
	_particleNodeState->boidSimShader.SetUniform("goalPos", goalPos);
	GFXBufferBindBase(state->bufferParticles, 0);
	GFXBufferBindBase(state->bufferParticleIndices, 1);
	GFXBufferBindBase(state->bufferCellIndices, 2);
	GFXBufferBindBase(state->bufferCellOffset, 3);
	GFXCompute(amount / WORKGROUP_SIZE, 1, 1);
	GFXMemBarrier(GFX_BARRIER_STORAGE);
	_particleNodeState->boidSimShader.End();
	GFXFinish();

	// COPY PARTICLES TO MESH
	/* state->particles = (Particle*)GFXBufferMapRange(state->bufferParticles, */
													  /* PARTICLE_AMOUNT * sizeof(Particle)); */
	state->particles = (Particle*)GFXBufferMap(state->bufferParticles);
	for(i32 i = 0; i < amount; i++) {
		Vertex v = {};
		v.vx = state->particles[i].pos.x;
		v.vy = state->particles[i].pos.y;
		v.vz = state->particles[i].pos.z;
		output->SetVertex(i, v);

		/* if(i == 100) { */
		/* 	DebugLog("pos: x%f, y%f, z%f", */
		/* 			 state->particles[i].pos.x, */
		/* 			 state->particles[i].pos.y, */
		/* 			 state->particles[i].pos.z); */
		/* 	DebugLog("vel x%f, y%f, z%f", */
		/* 			 state->particles[i].vel.x, */
		/* 			 state->particles[i].vel.y, */
		/* 			 state->particles[i].vel.z); */
		/* } */
	}
	GFXBufferUnmap(state->bufferParticles);

	self->changed = true;
}

// setup is used for everything after creation.
// this is needed since we dont call create when loading
void ParticleSetup(Node *self)
{
	ParticleNode particle = {};
	particle.bufferParticles = GFXBufferAdd(GFX_BUFFER_STORAGE);
	particle.bufferParticleIndices = GFXBufferAdd(GFX_BUFFER_STORAGE);
	particle.bufferCellIndices = GFXBufferAdd(GFX_BUFFER_STORAGE);
	particle.bufferCellOffset = GFXBufferAdd(GFX_BUFFER_STORAGE);
	self->extraHandle = _particleNodeState->particleNodes.Insert(particle);
}

ObjectHandle ParticleCreate()
{
	/*
	  minDist = distance before avoid kicks in
	  maxRange = "visual range" of particle
	 */
	FixedArray<NodeParameter> params = {
		NodeParameter("amount", 1024),
		NodeParameter("avg", 0.01),
		NodeParameter("avoid", 0.02),
		NodeParameter("center", 0.01),
		NodeParameter("goal", 0.02),
		NodeParameter("maxRange", 40.0),
		NodeParameter("minDist", 20.0),
		NodeParameter("speed", 0.01),
		NodeParameter("limit", 0.1),
	};

	FixedArray<NodeInput> inputs = {
	};

	return AddNode(DATA_MESH, params, inputs);
}

void AddParticleNodes()
{
	String workgroupSizeStr("#define WORKGROUP_SIZE ");
	workgroupSizeStr.Concat(WORKGROUP_SIZE);
	workgroupSizeStr.Concat("\n");

	String totalGridCellCountStr("#define TOTAL_GRID_CELL_COUNT ");
	totalGridCellCountStr.Concat(TOTAL_GRID_CELL_COUNT);
	totalGridCellCountStr.Concat("\n");

	VMArray<String> workgroupAndTotalGrid = {workgroupSizeStr, totalGridCellCountStr};
	VMArray<String> workGroupSize = {workgroupSizeStr, totalGridCellCountStr};

	_particleNodeState = new ParticleNodeState();
	_particleNodeState->cellIndexShader = Shader("assets/shaders/particles/cellindex.comp", workgroupAndTotalGrid);

	_particleNodeState->sortShader = Shader("assets/shaders/particles/sort.comp", workGroupSize);

	_particleNodeState->offsetShader = Shader("assets/shaders/particles/offsetShader.comp", workGroupSize);
	_particleNodeState->boidSimShader = Shader("assets/shaders/particles/boidsim.comp", workgroupAndTotalGrid);


	AddNodeConstructor(String("particle"), ParticleOp, ParticleCreate, ParticleSetup);
}
