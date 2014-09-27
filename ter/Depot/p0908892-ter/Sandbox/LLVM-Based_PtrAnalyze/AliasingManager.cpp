//===--- AliasingManager.cpp ------------------------------------*- C++ -*-===//
//                                    AUTHOR
// Name: Christophe Bacara
// Mail: christophe dot bacara at etudiant dot univ hyphen lille1 dot fr
// 
// Resp.: Laure Gonnord - INRIA/IRCICA - DaRT/Emeraude
//
//===----------------------------------------------------------------------===//
//
// 
//
//===----------------------------------------------------------------------===//

class AliasingManager {
	private:
	typedef enum MatrixKind_e {
		MK_PointsToMatrix,
		MK_SymetricalMatrix,
		MK_ClosedMatrix
	} MatrixKind;

	typedef bool** boolean_matrix;

	map<VarDecl*, PointerInfo*> m_pinfosMap;
	map<VarDecl*, int> m_indexesMap;
	map<MatrixKind, boolean_matrix> m_matricesMap;

	int m_dimension;

	bool m_matrixBuilt;
	bool m_matrixChanged;

	public:
	AliasingManager();
	~AliasingManager();

	void setPointerInfoMap(map<VarDecl*, PointerInfo*> &pinfos);

	void alias(VarDecl *lhs, VarDecl* rhs, bool init);
	void unset(VarDecl *VD);

	bool areAliases(VarDecl* VD1, VarDecl *VD2);

	void dumpAll();
	void dumpMatrix(boolean_matrix matrix);

	private:
	void build();
	void checkChanges();
	void initMatrix(MatrixKind kind);
	void computeClosure();
};

AliasingManager::AliasingManager() {
	m_dimension = 0;

	m_matricesMap[MK_PointsToMatrix] = NULL;
	m_matricesMap[MK_SymetricalMatrix] = NULL;
	m_matricesMap[MK_ClosedMatrix] = NULL;

	m_matrixBuilt = false;
	m_matrixChanged = false;
}

AliasingManager::~AliasingManager() {
	typedef map<MatrixKind, boolean_matrix>::iterator it;

	for (it b = m_matricesMap.begin(), e = m_matricesMap.end(); b != e; ++b) {
		for (int line = 0; line < m_dimension; ++line)
			free(b->second[line]);
		free(b->second);
	}
}

void AliasingManager::setPointerInfoMap(map<VarDecl*, PointerInfo*> &pinfos) {
	if (m_matrixBuilt)
		return;

	m_pinfosMap = pinfos;
}

void AliasingManager::alias(VarDecl *lhs, VarDecl *rhs, bool init = false) {
	int x;
	int y;

	checkChanges();

	// Error checking
	if (m_pinfosMap.find(lhs) == m_pinfosMap.end())
		llvm_unreachable("error alias(): lhs not found into pmap");
	if (m_pinfosMap[lhs]->isAnonymousPointer())
		llvm_unreachable("error alias(): lhs is anonymous pointer");
	if (m_pinfosMap.find(rhs) == m_pinfosMap.end())
		llvm_unreachable("error alias(): rhs not found into pmap");

	// Getting lhs index
	x = m_indexesMap[lhs];

	// If this is not initialization of lhs...
	if (!init) {
		if (areAliases(lhs, rhs))
			return;
		else
			m_pinfosMap[lhs]->setNonConst();
	}

	// Unset current matrix configuration for lhs
	unset(lhs);

	// Getting target of rhs if it's a declared pointer
	if (!m_pinfosMap[rhs]->isAnonymousPointer() && !m_pinfosMap[rhs]->isParmVar())
		rhs = m_pinfosMap[rhs]->getTarget();

	// Getting rhs indexes
	y = m_indexesMap[rhs];

	// Changing matrix configuration
	m_matricesMap[MK_PointsToMatrix][x][y] = true;
	m_matrixChanged = true;

	// Setting lhs new target
	m_pinfosMap[lhs]->setTarget(rhs);
}

void AliasingManager::unset(VarDecl* VD) {
	int line;
	int col;

	checkChanges();

	line = m_indexesMap[VD];

	for (col = 0; col < m_dimension; ++col) {
		if (line != col && m_matricesMap[MK_PointsToMatrix][line][col]) {
			m_matricesMap[MK_PointsToMatrix][line][col] = false;
			break;
		}
	}

	m_matrixChanged = true;
}

bool AliasingManager::areAliases(VarDecl *VD1, VarDecl *VD2) {
	int x;
	int y;

	checkChanges();

	x = m_indexesMap[VD1];
	y = m_indexesMap[VD2];

	return m_matricesMap[MK_ClosedMatrix][x][y];
}

void AliasingManager::build() {
	typedef map<VarDecl*, PointerInfo*>::iterator pinfos_it;
	typedef map<MatrixKind, boolean_matrix>::iterator matrix_it;

	int min_index;
	int max_index;
	int sz;

	// Setting matrices' dimension
	m_dimension = m_pinfosMap.size();

	// Setting min/max index
	min_index = 0;
	max_index = m_dimension - 1;

	// Attributing index to each VarDecl*/PointerInfo*
	for (pinfos_it b = m_pinfosMap.begin(), e = m_pinfosMap.end(); b != e; ++b) {
		if (b->second->isAnonymousPointer())
			m_indexesMap[b->first] = max_index--;
		else
			m_indexesMap[b->first] = min_index++;
	}

	// Computing the size of one line in the matrix
	sz = sizeof(bool) * m_dimension;

	// Allocating matrices
	for (matrix_it b = m_matricesMap.begin(), e = m_matricesMap.end(); b != e; ++b) {
		// Allocating a bool** for each line
		b->second = (bool**)malloc(sizeof(bool*) * m_dimension);
		// Allocating each line of the matrix
		for (int i = 0; i < m_dimension; ++i)
			b->second[i] = (bool*)malloc(sizeof(bool) * sz);

		// Setting default values (aka, "identity")
		for (int line = 0; line < m_dimension; ++line)
			for (int col = 0; col < m_dimension; ++col)
				b->second[line][col] = (line == col);
	}

	m_matrixBuilt = true;
	m_matrixChanged = true;
}

void AliasingManager::checkChanges() {
	if (!m_matrixBuilt)
		build();

	if (m_matrixChanged)
		computeClosure();
}

void AliasingManager::initMatrix(MatrixKind kind) {
	switch (kind) {
		// Initialize symetrical matrix
		case MK_SymetricalMatrix:
			// Setting each element of the symetric matrix
			for (int line = 0; line < m_dimension; line++)
				for (int col = 0; col < m_dimension; col++)
				if (m_matricesMap[MK_PointsToMatrix][line][col] || m_matricesMap[MK_PointsToMatrix][col][line])
					m_matricesMap[MK_SymetricalMatrix][line][col] = true;
				else
					m_matricesMap[MK_SymetricalMatrix][line][col] = false;
			break;

		// Initialize closed matrix
		case MK_ClosedMatrix:    
			// Setting each element of the closed matrix on symetric one's values
			for (int line = 0; line < m_dimension; ++line)
				for (int col = 0; col < m_dimension; ++col)
					m_matricesMap[MK_ClosedMatrix][line][col] = m_matricesMap[MK_SymetricalMatrix][line][col];
			break;

		// Others
		default:
			llvm_unreachable("erreur initMatrix(): unknown matrix kind");
			break;
	}
}

void AliasingManager::computeClosure() {
	// Initialization of matrices
	initMatrix(MK_SymetricalMatrix);
	initMatrix(MK_ClosedMatrix);

	// Efficient closure algorithm
	// http://www.enseignement.polytechnique.fr/informatique/profs/
	//                                  Jean-Jacques.Levy/poly/main5/node5.html
	for (int x = 0, n = m_dimension; x < n; x++)
		for (int u = 0; u < n; u++)
			if (m_matricesMap[MK_ClosedMatrix][u][x])
				for (int v = 0; v < n; v++)
					if (m_matricesMap[MK_ClosedMatrix][x][v])
						m_matricesMap[MK_ClosedMatrix][u][v] = true;

	m_matrixChanged = false;
}

void AliasingManager::dumpAll() {
	typedef map<VarDecl*, int>::iterator it_p;
	typedef map<MatrixKind, boolean_matrix>::iterator it_m;

	// Output all pointers within map
	for (it_p b = m_indexesMap.begin(), e = m_indexesMap.end(); b != e; ++b) {
		outs() << "\t" << b->second<< " => "<< b->first->getNameAsString()<< "\n";
	}

	// Output aliases matrix
	for (it_m b = m_matricesMap.begin(), e = m_matricesMap.end(); b != e; ++b) {
		// Switch matrix' kind
		switch (b->first) {
			case MK_PointsToMatrix:
				outs() << "# Points-to & Pointed-by Matrix:\n";
				break;
			case MK_SymetricalMatrix:
				outs() << "# Symetrical Matrix:\n";
				break;
			case MK_ClosedMatrix:
				outs() << "# Aliases Matrix:\n";
				break;
		}

		dumpMatrix(b->second);

		outs() << "----------------------------------------\n";
	}
}

void AliasingManager::dumpMatrix(boolean_matrix matrix) {
	for (int line = 0; line < m_dimension; line++) {
		outs() << "\t";

		if (m_dimension > 1) {
			if (line == 0)
				outs() << "/ ";
			else if (line == m_dimension - 1)
				outs() << "\\ ";
			else
				outs() << "| ";
		}

		for (int col = 0; col < m_dimension; col++)
			outs() << (matrix[line][col] ? "1 " : "0 ");

		if (m_dimension > 1) {
			if (line == 0)
				outs() << "\\";
			else if (line == m_dimension - 1)
				outs() << "/";
			else
				outs() << "|";
		}

		outs() << "\n";
	}
}
