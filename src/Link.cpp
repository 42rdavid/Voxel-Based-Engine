
#include "Link.hpp"
#include "Chunk.hpp"
#include "Block.hpp"
#include "Biome.hpp"

Link::Link(void) : Octree()
{
	for (int i = 0; i < CHD_MAX; ++i)
		this->_children[i] = NULL;
}

Link::Link(float const &x, float const &y, float const &z, float const &s) : Octree(x, y, z, s)
{
	for (int i = 0; i < CHD_MAX; ++i)
		this->_children[i] = NULL;
}

Link::Link(Link const &src) : Octree(src)
{
	for (int i = 0; i < CHD_MAX; ++i)
		this->_children[i] = NULL;
}

Link::~Link(void)
{
	for (int i = 0; i < CHD_MAX; ++i)
	{
		if (this->_children[i] != NULL)
			delete this->_children[i];
		this->_children[i] = NULL;
	}
}

// -------------------------------------------------------------------
// Creates a child with octree state specified
// -------------------------------------------------------------------
void
Link::createChild(uint32_t const &i, float const &x, float const &y, float const &z, float const &s, int32_t const &state, Vec3<float> const &c)
{
	switch (state)
	{
		case CHUNK:
			this->_children[i] = new Chunk(x, y, z, s);
			break;
		case BLOCK:
			this->_children[i] = new Block(c);
			this->_children[i]->setCube(x, y, z, s);
			break;
		case BIOME:
			this->_children[i] = new Biome(x, y, z, s);
			break;
		default:
			this->_children[i] = new Link(x, y, z, s);
	}
	this->_children[i]->setParent(this);
}
// -------------------------------------------------------------------

// -------------------------------------------------------------------
// Search the octree with a point and returns a pointer
// on the deepest child found.
// -------------------------------------------------------------------
Octree *
Link::search(float const &x, float const &y, float const &z)
{
	Octree *		child;
	int				i;

	if (this->_cube.vertexInside(x, y, z))
	{
		child = NULL;
		for (i = 0; i < CHD_MAX; ++i)
		{
			if (this->_children[i] != NULL)
			{
				if ((child = this->_children[i]->search(x, y, z)) != NULL)
					break;
			}
		}
		return (child == NULL ? this : child);
	}
	else
		return (NULL);
}
// -------------------------------------------------------------------

Octree *
Link::search(float const &x, float const &y, float const &z, int const &state)
{
	Octree *		child;
	int				i;

	if (this->_cube.vertexInside(x, y, z))
	{
		if (this->_state == state)
			return (this);
		child = NULL;
		for (i = 0; i < CHD_MAX; ++i)
		{
			if (this->_children[i] != NULL)
			{
				if ((child = this->_children[i]->search(x, y, z)) != NULL)
					break;
			}
		}
		return (child == NULL ? this : child);
	}
	else
		return (NULL);
}

// -------------------------------------------------------------------
// Insert an Octree in another
// -------------------------------------------------------------------
Octree *
Link::insert(float const &x, float const &y, float const &z, uint32_t const &depth, int32_t const &state, Vec3<float> const &c)
{
	// size never changes for children.
	float const		s = this->_cube.getS() / 2.0f;
	float			nx;
	float			ny;
	float			nz;

	if (depth == 0)
	{
		// max depth reached, put values here
		this->setState(state);
		// std::cerr << "insert deepest: " << this << std::endl;
		return (this);
	}
	else if (depth > 0)
	{
		int				i;

		for (i = 0; i < CHD_MAX; ++i)
		{
			if (this->_children[i] == NULL)
			{
				/*
				** precomputing child coordinates in order to check if the vertex is inside.
				** new_dim = dim + coefficient * size;
				*/
				nx = this->_cube.getX() + ((i >> 0) & MASK_1) * s;
				ny = this->_cube.getY() + ((i >> 1) & MASK_1) * s;
				nz = this->_cube.getZ() + ((i >> 2) & MASK_1) * s;
				if (x >= nx && x < nx + s && y >= ny && y < ny + s && z >= nz && z < nz + s)
				{
					if ((depth - 1) == 0)
						this->createChild(i, nx, ny, nz, s, state, c);
					else
						this->createChild(i, nx, ny, nz, s, -1, c);
					return (this->_children[i]->insert(x, y, z, depth - 1, state, c));
				}
			}
			else if (this->_children[i]->getCube()->vertexInside(x, y, z))
				return (this->_children[i]->insert(x, y, z, depth - 1, state, c));
		}
	}
	// std::cerr << "not created, depth: " << depth << std::endl;
	return (NULL);
}
// -------------------------------------------------------------------

void
Link::render(void) const
{
	int				i;

	for (i = 0; i < CHD_MAX; ++i)
	{
		if (this->_children[i] != NULL)
			this->_children[i]->render();
	}
}

Octree *
Link::getChild(uint32_t const &i)
{
	return (this->_children[i]);
}

void
Link::setChild(uint32_t const &i, Octree *const child)
{
	this->_children[i] = child;
}

Link &
Link::operator=(Link const &rhs)
{
	if (this != &rhs)
	{
		// copy members here
	}
	return (*this);
}

std::ostream &
operator<<(std::ostream &o, Link const &i)
{
	o	<< "Link: " << &i;
	return (o);
}
