#ifndef __gpuProgram_H
#define __gpuProgram_H 1

enum ShaderType
{
	Fragment = 0x01, // fragment (pixel) program
	Vertex = 0x02, // vertex program
	Geometry = 0x04, // geometry program
	TessellationEvaluation = 0x08, // tesselation evaluation (domain) program
	TessellationControl = 0x10, // tesselation control (hull) program
	Compute = 0x20 // compute program
};


class GPUProgram
{
public:
	virtual ~GPUProgram() = 0;

	virtual void shutdown(void) = 0;

	virtual bool compileShader(ShaderType type, const char* const source[]) = 0;

	virtual void link(void) = 0;

	virtual void bind(void) = 0;
	
	virtual void unbind(void) = 0;

	virtual unsigned char getProgramType(void) const = 0;

};




#endif // !__gpuProgram_H
