
#ifndef		BIOME_HPP
# define	BIOME_HPP

# include <iostream>
# include "Constants.hpp"
# include "Octree.hpp"

class Biome : public Octree
{
public:
	Biome(void);
	Biome(float const &x, float const &y, float const &z, float const &s);
	Biome(Biome const &src);
	virtual ~Biome(void);

	Biome				&operator=(Biome const &rhs);
};

std::ostream				&operator<<(std::ostream &o, Biome const &i);

#endif