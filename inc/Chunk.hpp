
#ifndef CHUNK_HPP
# define CHUNK_HPP

# include <iostream>
# include <vector>
# include "Constants.hpp"
# include "Link.hpp"
# include "Mesh.hpp"

class Chunk : public Link
{
private:
	bool							_generating;
	bool							_generated;
	bool							_renderDone;
	bool							_stopGenerating;
	bool							_removable;

public:
	Mesh							mesh;
	Vec3<uint8_t>					pos; // position relative to other chunks

	Chunk(void);
	Chunk(float const &x, float const &y, float const &z, float const &s);
	virtual ~Chunk(void);

	virtual	void					render(Core &core) const;
	virtual	void					renderLines(Core &core) const;
	virtual	void					renderRidges(Core &core) const;
	virtual void					deleteChild(Octree *child);
	virtual Chunk *					getChunk(void);

	bool const &					getGenerating(void);
	bool const &					getGenerated(void);
	bool const &					getRenderDone(void);
	bool const &					getStopGenerating(void);
	bool const &					getRemovable(void);

	void							setGenerating(bool const &val);
	void							setGenerated(bool const &val);
	void							setRenderDone(bool const &val);
	void							setStopGenerating(bool const &val);
	void							setRemovable(bool const &val);

	Chunk							&operator=(Chunk const &rhs);

private:
	Chunk(Chunk const &src);
};

std::ostream						&operator<<(std::ostream &o, Chunk const &i);

#endif
