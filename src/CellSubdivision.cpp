#include    <iostream>

#include	"CellSubdivision.h"
#include	"Particle.hpp"

/*
@param width width of the box
@param height height of the box
@param rc cut-off distance
*/
CellSubdivision::CellSubdivision(const double width, const double height, const double rc):
	m_origin(0.0,0.0,0.0), m_boxHeight(height), m_boxWidth(width), m_numCellsX(floor(width/rc)), m_numCellsY(floor(height/rc)){

	m_cellWidth = (width/static_cast<double>(m_numCellsX));
	m_cellHeight = (height/static_cast<double>(m_numCellsY));
	//create cell array
	m_cells.resize(m_numCellsX*m_numCellsY);

}

/*
@param pos position of particle
@param id id of particle
@return cell id
*/
unsigned int	CellSubdivision::InsertParticle(const Vector& pos, const unsigned int id){
	Vector a(pos - m_origin);
	unsigned int x(floor(a[0]/m_cellWidth));
	unsigned int y(floor(a[1]/m_cellHeight));
	unsigned int cellId(GetCell(x,y));
	m_cells[cellId].insert(id);
	return cellId;
}

/*
@param cellId id of the cell the particle is in
@param id id of the particle
*/
void	CellSubdivision::DeleteParticle(const unsigned int cellId, const unsigned int id){
	m_cells[cellId].erase(id);
}

/*
@param cellId id of the cell the particle is in
@param id id of the particle
@return list iterator of a list of nearest neighbours
*/
list<unsigned int>::iterator CellSubdivision::GetNeighbours(const unsigned int cellId, const unsigned int id){
    m_neighbours.clear();

    int x,y;
    int a,b;
    GetCoordinates(cellId, x, y);

    set<unsigned int>::iterator it;

    for (int i = x-1; i<x+2; i++)
        for (int j = y-1; j<y+2; j++){
            a = i;
            b = j;
            NormaliseCoordinates(a,b);
            for (it=m_cells[GetCell(a,b)].begin(); it!=m_cells[GetCell(a,b)].end(); it++)
                if (*it!=id) m_neighbours.push_back(*it);
        }

    return m_neighbours.begin();
}

void	CellSubdivision::GetCellWallRight(const int cellId, Vector& point, Vector& normal){
	normal = Vector(-1,0,0);
	int x;
	int y;
	GetCoordinates(cellId, x, y);
	point = Vector( (x+1.0) * m_cellWidth,0,0);
}
void	CellSubdivision::GetCellWallLeft(const int cellId, Vector& point, Vector& normal){
	normal = Vector(1,0,0);
	int x;
	int y;
	GetCoordinates(cellId, x, y);
	point = Vector( (x) * m_cellWidth,0,0);
}
void	CellSubdivision::GetCellWallBottom(const int cellId, Vector& point, Vector& normal){
	normal = Vector(0,1,0);
	int x;
	int y;
	GetCoordinates(cellId, x, y);
	point = Vector( 0, y * m_cellHeight,0);
}
void	CellSubdivision::GetCellWallTop(const int cellId, Vector& point, Vector& normal){
	normal = Vector(0,-1,0);
	int x;
	int y;
	GetCoordinates(cellId, x, y);
	point = Vector( 0, (y+1) * m_cellHeight,0);
}
