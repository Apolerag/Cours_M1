//===--- Utils.cpp ----------------------------------------------*- C++ -*-===//
//                                    AUTHOR
// Name: Christophe Bacara
// Mail: christophe dot bacara at etudiant dot univ hyphen lille1 dot fr
// 
// Resp.: Laure Gonnord - INRIA/IRCICA - DaRT/Emeraude
//
///===----------------------------------------------------------------------===//
//
// This files contains utility methods.
// 
//===----------------------------------------------------------------------===//
namespace util {
	/* Template: getVarDeclFromDeclRefExpr
	* Tente de récupérer un pointeur vers la définition de la variable
	* référencée par DRE. Si la méthode est paramétrée explicitement, le type
	* de la variable recherché doit correspondre.
	*
	* Retourne NULL en cas d'erreur.
	*/
	template <typename T = void>
	VarDecl* getVarDeclFromDeclRefExpr(clang::DeclRefExpr *DRE);
	template <typename T>
	VarDecl* __builtin_getVarDeclFromDeclRefExpr(clang::DeclRefExpr *DRE);
	template <>
	VarDecl* __builtin_getVarDeclFromDeclRefExpr<void>(clang::DeclRefExpr *DRE);

	/* Template: getVarDeclFromAddrOfOperator
	* Tente de récupérer un pointeur vers la définition de la variable
	* à laquelle l'opérateur & est appliqué. Si la méthode est paramétrée
	* explicitement, le type de la variable recherché doit correspondre.
	*
	* Retourne NULL en cas d'erreur.
	*/
	template <typename T=void>
	VarDecl* getVarDeclFromAddrOfOperator(clang::UnaryOperator *UOP);
	template <typename T>
	VarDecl* __builtin_getVarDeclFromAddrOfOperator(clang::UnaryOperator *UOP);
	template <>
	VarDecl* __builtin_getVarDeclFromAddrOfOperator<void>(clang::UnaryOperator *UOP);

	/* Template: getVarDeclFromIncDecOperator
	* Tente de récupérer un pointeur vers la définition de la variable
	* à laquelle est appliqué un opérateur ++ ou --. Si la méthode est
	* paramétrée explicitement, le type de la variable recherché doit 
	* correspondre.
	*
	* Retourne NULL en cas d'erreur.
	*/
	template <typename T=void>
	VarDecl* getVarDeclFromIncDecOperator(clang::UnaryOperator *UOP);
	template <typename T>
	VarDecl* __builtin_getVarDeclFromIncDecOperator(clang::UnaryOperator *UOP);
	template <>
	VarDecl* __builtin_getVarDeclFromIncDecOperator<void>(clang::UnaryOperator *UOP);

	/* Template: getVarDeclFromDerefOperator
	* Tente de récupérer un pointeur vers la définition de la variable
	* à laquelle est appliqué un opérateur *. Si la méthode est
	* paramétrée explicitement, le type de la variable recherché doit 
	* correspondre.
	*
	* Retourne NULL en cas d'erreur.
	*/
	template <typename T=void>
	VarDecl* getVarDeclFromDerefOperator(clang::UnaryOperator *UOP);
	template <typename T>
	VarDecl* __builtin_getVarDeclFromDerefOperator(clang::UnaryOperator *UOP);
	template <>
	VarDecl* __builtin_getVarDeclFromDerefOperator<void>(clang::UnaryOperator *UOP);

	/* getTypePtrFromVarDecl
	* Renvoit un pointeur vers le type correspondant à la variable spécifiée.
	*/
	const Type *getTypePtrFromVarDecl(clang::VarDecl * VD);

	/* isIntegerEvaluatedAsZero
	* Return true if E is an IntegerLitteral of value 0.
	*/
	bool isIntegerEvaluatedAsZero(clang::Expr *E, clang::ASTContext &astcontext);
};

//===----------------------------------------------------------------------===//
// util::getVarDeclFromDeclRefExpr
//===----------------------------------------------------------------------===//
template <typename T>
VarDecl* util::getVarDeclFromDeclRefExpr(clang::DeclRefExpr *DRE) {
	return __builtin_getVarDeclFromDeclRefExpr<T>(DRE);
}

template <typename T>
VarDecl* util::__builtin_getVarDeclFromDeclRefExpr(clang::DeclRefExpr *DRE) {
	VarDecl *VD;

	if ((VD = dyn_cast<VarDecl>(DRE->getDecl())) && (isa<T>(getTypePtrFromVarDecl(VD))))
		return VD;

	return NULL;
}

template <>
VarDecl* util::__builtin_getVarDeclFromDeclRefExpr<void>(clang::DeclRefExpr *DRE) {
	return dyn_cast<VarDecl>(DRE->getDecl());  
}

//===----------------------------------------------------------------------===//
// util::getVarDeclFromAddrOfOperator
//===----------------------------------------------------------------------===//
template <typename T>
VarDecl* util::getVarDeclFromAddrOfOperator(clang::UnaryOperator *UOP) {
	return __builtin_getVarDeclFromAddrOfOperator<T>(UOP);
}

template <typename T>
VarDecl* util::__builtin_getVarDeclFromAddrOfOperator(clang::UnaryOperator *UOP) {
	VarDecl *VD;

	if ((VD = __builtin_getVarDeclFromAddrOfOperator<void>(UOP)) && isa<T>(getTypePtrFromVarDecl(VD)))
		return VD;

	return NULL;
}

template <>
VarDecl* util::__builtin_getVarDeclFromAddrOfOperator<void>(clang::UnaryOperator *UOP) {
	DeclRefExpr *DRE;
	VarDecl *VD;

	if (UOP->getOpcode() == UO_AddrOf && (DRE = dyn_cast<DeclRefExpr>(UOP->getSubExpr()->IgnoreParenCasts())) && (VD = dyn_cast<VarDecl>(DRE->getDecl())))
		return VD;

	return NULL;
}

//===----------------------------------------------------------------------===//
// util::getVarDeclFromIncDecOperator
//===----------------------------------------------------------------------===//
template <typename T>
VarDecl* util::getVarDeclFromIncDecOperator(clang::UnaryOperator *UOP) {
	return __builtin_getVarDeclFromIncDecOperator<T>(UOP);
}

template <typename T>
VarDecl* util::__builtin_getVarDeclFromIncDecOperator(clang::UnaryOperator *UOP) {
	VarDecl *VD;

	if ((VD = __builtin_getVarDeclFromIncDecOperator<void>(UOP)) && isa<T>(getTypePtrFromVarDecl(VD)))
		return VD;

	return NULL;
}

template <>
VarDecl* util::__builtin_getVarDeclFromIncDecOperator<void>(clang::UnaryOperator *UOP) {
	DeclRefExpr *DRE;
	VarDecl *VD;

	if (UOP->isIncrementDecrementOp() && (DRE = dyn_cast<DeclRefExpr>(UOP->getSubExpr()->IgnoreParenCasts())) && (VD = dyn_cast<VarDecl>(DRE->getDecl())))
		return VD;

	return NULL;
}

//===----------------------------------------------------------------------===//
// util::getVarDeclFromDerefOperator
//===----------------------------------------------------------------------===//
template <typename T>
VarDecl* util::getVarDeclFromDerefOperator(clang::UnaryOperator *UOP) {
	return __builtin_getVarDeclFromDerefOperator<T>(UOP);
}

template <typename T>
VarDecl* util::__builtin_getVarDeclFromDerefOperator(clang::UnaryOperator *UOP) {
	VarDecl *VD;

	if ((VD = __builtin_getVarDeclFromDerefOperator<void>(UOP)) && isa<T>(getTypePtrFromVarDecl(VD)))
		return VD;

	return NULL;
}

template <>
VarDecl* util::__builtin_getVarDeclFromDerefOperator<void>(clang::UnaryOperator *UOP) {
	DeclRefExpr *DRE;
	VarDecl *VD;

	if (UOP->getOpcode() == UO_Deref	&& (DRE = dyn_cast<DeclRefExpr>(UOP->getSubExpr()->IgnoreParenCasts())) && (VD = dyn_cast<VarDecl>(DRE->getDecl())))
		return VD;

	return NULL;
}

//===----------------------------------------------------------------------===//

const Type *util::getTypePtrFromVarDecl(clang::VarDecl * VD) {
	return VD->getTypeSourceInfo()->getType().getTypePtr();
}

bool util::isIntegerEvaluatedAsZero(Expr *E, ASTContext &astcontext) {
	IntegerLiteral *IL;
	APSInt result;

	if ((IL = dyn_cast<IntegerLiteral>(E)) && IL->EvaluateAsInt(result, astcontext) && result == 0)
		return true;

	return false;
}
