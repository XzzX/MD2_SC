#ifndef CELLSUBDIVISION_HPP
#define CELLSUBDIVISION_HPP

#include	<vector>
#include	<set>
#include    <list>

#include	"Vector.hpp"
#include	"Particle.hpp"

using namespace std;

class CellSubdivision {
    private:
		///origin of the box
		Vector	m_origin;
		///Height of the Box
		double m_boxHeight;
		///Width of the Box
		double m_boxWidth;
		///Cut-Off distance
		double m_cellWidth;
		double m_cellHeight;

		///number of cells in x direction
		unsigned int	m_numCellsX;
		///number of cells in y direction
		unsigned int	m_numCellsY;

		///Cell content
		vector< set<unsigned int> > 	m_cells;
		///list of nearest neighbours. will be changed with GetNeighbours
		list<unsigned int>   m_neighbours;
		///return 1D cell id from 2D coordinates
		unsigned int	GetCell(const int x, const int y); //inline

		void	GetCoordinates(const unsigned int cellId, unsigned int& x, unsigned int& y); //inline
		void	GetCoordinates(const unsigned int cellId, int& x, int& y); //inline
        void	NormaliseCoordinates(int& x, int& y);
	public:
		void	GetCellWallRight(const int cellId, Vector& point, Vector& normal);
		void	GetCellWallLeft(const int cellId, Vector& point, Vector& normal);
		void	GetCellWallBottom(const int cellId, Vector& point, Vector& normal);
		void	GetCellWallTop(const int cellId, Vector& point, Vector& normal);

		unsigned int GetNumCellsX(); //inline
		unsigned int GetNumCellsY(); //inline
		Color	GetCellColor(const unsigned int cellId); //inline

        inline list<unsigned int>::iterator   GetNeighbourBegin() {return m_neighbours.begin();};
        inline list<unsigned int>::iterator   GetNeighbourEnd() {return m_neighbours.end();};

		///constructor
		CellSubdivision(const double width, const double height, const double rc);

		///insert particle in corresponding cell
		unsigned int	InsertParticle(const Vector& pos, const unsigned int id);
		///delete particle from cell
		void	DeleteParticle(const unsigned int cellId, const unsigned int id);

		///return list iterator of neighbours
		list<unsigned int>::iterator GetNeighbours(const unsigned int cellId, const unsigned int id);
};

inline
unsigned int	CellSubdivision::GetCell(const int x, const int y) {
	int a = y*m_numCellsX+x;
	if (a<0) a=0;
	if (a>static_cast<int>(m_numCellsX*m_numCellsY)-1) a = static_cast<int>(m_numCellsX*m_numCellsY)-1;
	return a;
}

inline
void	CellSubdivision::GetCoordinates(const unsigned int cellId, unsigned int& x, unsigned int& y){
	x = cellId%GetNumCellsX();
	y = cellId/GetNumCellsX();
}
inline
void	CellSubdivision::GetCoordinates(const unsigned int cellId, int& x, int& y){
	x = cellId%GetNumCellsX();
	y = cellId/GetNumCellsX();
}

inline
void CellSubdivision::NormaliseCoordinates(int& x, int& y){
	if (x>=static_cast<int> (GetNumCellsX())) x-=GetNumCellsX(); else
		if (x<(0)) x+=GetNumCellsX();
	if (y>=static_cast<int> (GetNumCellsY())) y-=GetNumCellsY(); else
		if (y<0) y+=GetNumCellsY();
}

inline
unsigned int CellSubdivision::GetNumCellsX() {return m_numCellsX;}

inline
unsigned int CellSubdivision::GetNumCellsY() {return m_numCellsY;}

inline
Color	CellSubdivision::GetCellColor(const unsigned int cellId){
	Color a = Color(static_cast<double>(cellId % GetNumCellsX())/static_cast<double>(GetNumCellsX()),
				static_cast<double>(cellId / GetNumCellsX())/static_cast<double>(GetNumCellsY()),
				0.3);
	return a;
}

#endif
