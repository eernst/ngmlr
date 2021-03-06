/**
 * Contact: philipp.rescheneder@gmail.com
 */

#ifndef ALIGNMENT_MATRIX_FAST_
#define ALIGNMENT_MATRIX_FAST_

#include "IAlignment.h"
#include "Types.h"

/**
 * Used in directionMatrix
 */
//TOOD: replace #define by statics constants
#define CIGAR_M 0
#define CIGAR_I 1
#define CIGAR_D 2
#define CIGAR_N 3
#define CIGAR_S 4
#define CIGAR_H 5
#define CIGAR_P 6
#define CIGAR_EQ 7
#define CIGAR_X 8
#define CIGAR_STOP 10

namespace Convex {

class AlignmentMatrixFast {

public:

	typedef float Score;

	struct MatrixElement {

		/**
		 * Score of the current element
		 */
		Score score;
		/**
		 * Number of indels that preceded this cell (heuristic!!!)
		 */
		short indelRun;
		/**
		 * From where (up, left, diag) the score of this cell was derived
		 */
		char direction;

		MatrixElement() {
			score = 0.0f;
			indelRun = 0;
			direction = CIGAR_STOP;
		}
	};

	AlignmentMatrixFast();

	virtual ~AlignmentMatrixFast();

	int getCorridorOffset(int const y) const;

	int getCorridorLength(int const y) const;

	int getHeight() const;

	int getWidth() const;

	MatrixElement * getElement(int const x, int const y);

	MatrixElement * getElementEdit(int const x, int const y);


	//SAFE FUNCTIONS
	inline MatrixElement * getElementUp(int const x, int const y)
	{
		if (y < 0 || x < 0) {
		       return &empty;
		}
		       
		if (x < 0) {
		       return &empty;
		}
		if (x < lastCorridor.offset || x >= (lastCorridor.offset + lastCorridor.length)) {
		       return &empty;
		}


		return lastLine + (x - lastCorridor.offset);
	}

	inline MatrixElement * getElementUpUnprotected(int const x, int const y)
	{
		return lastLine + (x - lastCorridor.offset);
	}


	inline MatrixElement * getElementCurr(int const x, int const y)
	{
		if (y < 0 || x < 0) {
		       return &empty;
		}

		
		if (x < currentCorridor.offset || x >= (currentCorridor.offset + currentCorridor.length)) {
		       return &empty;
		}
		if (x < 0) {
		       return &empty;
		}
		return currentLine + (x - currentCorridor.offset);
	}

	inline MatrixElement * getElementCurrUnprotected(int const x, int const y)
	{
		return currentLine + (x - currentCorridor.offset);
	}

	inline MatrixElement * getElementEditCurr(int const x, int const y)
	{
		if (y < 0 || x < 0) {
		       return &empty;
		}


		if (x < currentCorridor.offset || x >= (currentCorridor.offset + currentCorridor.length)) {
		       return &empty;
		}

		return currentLine + (x - currentCorridor.offset);
	}

	//END SAFE FUNCTIONS

	//FAST FUNCTIONS
/*	inline MatrixElement * getElementUpFast(int const x, int const y)
	{
		throw "";
		return lastLine + (x - lastCorridor.offset);
	}

	inline MatrixElement * getElementCurrFast(int const x, int const y)
	{
		throw "";
		return currentLine + (x - currentCorridor.offset);
	}

	inline MatrixElement * getElementEditCurrFast(int const x, int const y)
	{
		throw "";
		return currentLine + (x - currentCorridor.offset);
	}

	inline char * getDirectionCurrFast(int const x, int const y) {
		throw "";
		CorridorLine line = corridorLines[y];
		return directionMatrix + line.offsetInMatrix + (x - line.offset);
	}*/

	//END FAST FUNCTIONS


	char * getDirection(int const x, int const y);

	Score getScore(int const x, int const y);

	/**
	 * Deletes allocated memory.
	 * Must be used after alignment computation
	 */
	void clean();

	/**
	 * Allocates memory and sets up the corridor layout
	 * Has to be called before filling the matrix
	 */
	void prepare(int const width, int const height, CorridorLine * corridor,
			int const corridorHeight);

	/**
	 * Swaps current and last line.
	 * Has to be used after currentLine is filled
	 */
	bool prepareLine(int const y);

	/**
	 * Prints matrix to stderr
	 */
	void printMatrix(char const * refSeq, char const * qrySeg);

	/**
	 * Returns false if backtracking path
	 * is "too close" to the corridor borders
	 * (heuristics)
	 */
	bool validPath(int const x, int const y);

public:

	/**
	 * Default size of alignment (char) matrix
	 */
	uloc const defaultMatrixSize;

	/**
	 * Height of the alignment matrix
	 */
	int privateMatrixHeight;

	/**
	 * Width of the alignment matrix
	 */
	int privateMatrixWidth;

	/**
	 * Current size of the matrix. If an alignment
	 * requires more then defaultMatrixSize, the matrix
	 * is reallocated to fit the alignment computation.
	 * After finishing the alignment, the matrix is
	 * reallocated again with defaultMatrixSize.
	 */
	uloc privateMatrixSize;

	/**
	 * The row of the matrix that is filled.
	 * Only the current and last row are kept
	 * in memory. For backtracking only
	 * the directionMatrix is required
	 */
	MatrixElement * currentLine;

	/**
	 * Holds the information on
	 * where in the full alignment matrix
	 * the currentLine starts and ends
	 */
	CorridorLine currentCorridor;

	int currentY;

	/**
	 * The last row of the matrix that is
	 * required to fill the current line.
	 */
	MatrixElement * lastLine;

	/**
	 * Holds the information on
	 * where in the full alignment matrix
	 * the lastLine starts and ends
	 */
	CorridorLine lastCorridor;

	int lastY;

	/**
	 * Matrix that holds the backtracking pointers
	 * for the alignment computation.
	 */
	//TODO: reduce memory by fitting directions
	//of two cells into one char?
	char * directionMatrix;

	/**
	 * Empty matrix element. Is returned for
	 * all cells that are outside of the
	 * alignment corridor
	 */
	MatrixElement empty;

	/**
	 * Stop operation. Is returned for all
	 * cells that are outside of the alignment
	 * corridor
	 */
	char STOP;

	/**
	 * Set of corridorLines that
	 * defines the alignment corridor
	 */
	CorridorLine * corridorLines;

};

#endif /* ALIGNMENT_MATRIX_ */

}  // namespace Convex
