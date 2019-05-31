package rs.ac.bg.etf.pp1;

import java.util.HashMap;
import java.util.Map;

import rs.ac.bg.etf.pp1.CounterVisitor.FormParamCounter;
import rs.ac.bg.etf.pp1.CounterVisitor.VarCounter;
import rs.ac.bg.etf.pp1.ast.BooleanConst;
import rs.ac.bg.etf.pp1.ast.CharacterConst;
import rs.ac.bg.etf.pp1.ast.DesignArr;
import rs.ac.bg.etf.pp1.ast.DesignNoArr;
import rs.ac.bg.etf.pp1.ast.Designator;
import rs.ac.bg.etf.pp1.ast.DesignatorDec;
import rs.ac.bg.etf.pp1.ast.DesignatorEqual;
import rs.ac.bg.etf.pp1.ast.DesignatorInc;
import rs.ac.bg.etf.pp1.ast.Div;
import rs.ac.bg.etf.pp1.ast.ExprAddop;
import rs.ac.bg.etf.pp1.ast.ExprExpanded2;
import rs.ac.bg.etf.pp1.ast.FactorBoolean;
import rs.ac.bg.etf.pp1.ast.FactorCharacter;
import rs.ac.bg.etf.pp1.ast.FactorFunc;
import rs.ac.bg.etf.pp1.ast.FactorNewArrayElem;
import rs.ac.bg.etf.pp1.ast.FactorNumber;
import rs.ac.bg.etf.pp1.ast.FactorProc;
import rs.ac.bg.etf.pp1.ast.FormPar;
import rs.ac.bg.etf.pp1.ast.FuncCallNoArgs;
import rs.ac.bg.etf.pp1.ast.FuncCallWithArgs;
import rs.ac.bg.etf.pp1.ast.MethodDecl;
import rs.ac.bg.etf.pp1.ast.MethodTypeName;
import rs.ac.bg.etf.pp1.ast.MinusTermExp;
import rs.ac.bg.etf.pp1.ast.Mul;
import rs.ac.bg.etf.pp1.ast.Mulop;
import rs.ac.bg.etf.pp1.ast.NumberConst;
import rs.ac.bg.etf.pp1.ast.Plus;
import rs.ac.bg.etf.pp1.ast.PrintStatement;
import rs.ac.bg.etf.pp1.ast.ReadStatement;
import rs.ac.bg.etf.pp1.ast.ReturnEmptyStatement;
import rs.ac.bg.etf.pp1.ast.ReturnExprStatement;
import rs.ac.bg.etf.pp1.ast.SyntaxNode;
import rs.ac.bg.etf.pp1.ast.TermFactorial;
import rs.ac.bg.etf.pp1.ast.TermMulop;
import rs.ac.bg.etf.pp1.ast.VarDecl;
import rs.ac.bg.etf.pp1.ast.VisitorAdaptor;
import rs.etf.pp1.mj.runtime.Code;
import rs.etf.pp1.symboltable.Tab;
import rs.etf.pp1.symboltable.concepts.Obj;
import rs.etf.pp1.symboltable.concepts.Struct;

public class CodeGenerator extends VisitorAdaptor {

	private int varCount;

	private int paramCnt;

	private int mainPc;

	public int getMainPc() {
		return mainPc;
	}

	@Override
	public void visit(MethodTypeName MethodTypeName) {
		if ("main".equalsIgnoreCase(MethodTypeName.getI2())) {
			mainPc = Code.pc;
		}
		MethodTypeName.obj.setAdr(Code.pc);

		// Collect arguments and local variables.
		SyntaxNode methodNode = MethodTypeName.getParent();
		VarCounter varCnt = new VarCounter();
		methodNode.traverseTopDown(varCnt);
		FormParamCounter fpCnt = new FormParamCounter();
		methodNode.traverseTopDown(fpCnt);

		// Generate the entry.
		Code.put(Code.enter);
		Code.put(fpCnt.getCount());
		Code.put(varCnt.getCount() + fpCnt.getCount());
	}

	@Override
	public void visit(VarDecl VarDecl) {
		varCount++;
	}

	@Override
	public void visit(FormPar FormalParam) {
		paramCnt++;
	}

	@Override
	public void visit(MethodDecl MethodDecl) {
		Code.put(Code.exit);
		Code.put(Code.return_);
	}

	@Override
	public void visit(ReturnEmptyStatement ReturnExpr) {
		Code.put(Code.exit);
		Code.put(Code.return_);
	}

	@Override
	public void visit(ReturnExprStatement ReturnNoExpr) {
		Code.put(Code.exit);
		Code.put(Code.return_);
	}

	@Override
	public void visit(DesignatorEqual Assignment) {
		Code.store(Assignment.getDesignator().obj);
	}

	public void visit(DesignatorInc DesInc) {
		int kind = DesInc.getDesignator().obj.getKind();
		if (kind == Obj.Elem) {
			Code.put(Code.dup2);
			Code.load(DesInc.getDesignator().obj);
		}
		Code.loadConst(1);
		Code.put(Code.add);
		Code.store(DesInc.getDesignator().obj);
	}

	public void visit(DesignatorDec DesDec) {
		int kind = DesDec.getDesignator().obj.getKind();
		if (kind == Obj.Elem) {
			Code.put(Code.dup2);
			Code.load(DesDec.getDesignator().obj);
		}
		Code.loadConst(1);
		Code.put(Code.sub);
		Code.store(DesDec.getDesignator().obj);
	}

	public void visit(TermFactorial fact) {
		
		Code.put(Code.dup);
		Code.loadConst(1);
		Code.put(Code.sub);
		
	}
	
	@Override
	public void visit(NumberConst Const) {
		Code.load(new Obj(Obj.Con, "$", Const.struct, Const.getN1(), 0));
	}

	public void visit(CharacterConst Const) {
		Code.load(new Obj(Obj.Con, "$", Const.struct, Const.getC1(), 0));
	}

	public void visit(BooleanConst Const) {
		if (Const.getB1().equals("true")) {
			Code.load(new Obj(Obj.Con, "$", SemanticPass.boolType, 1, 0));
		} else {
			Code.load(new Obj(Obj.Con, "$", SemanticPass.boolType, 0, 0));
		}

	}

	public void visit(FactorNumber Const) {
		Code.load(new Obj(Obj.Con, "$", Const.struct, Const.getN1(), 0));
	}

	public void visit(FactorBoolean Const) {
		if (Const.getB1().equals("true")) {
			Code.load(new Obj(Obj.Con, "$", SemanticPass.boolType, 1, 0));
		} else {
			Code.load(new Obj(Obj.Con, "$", SemanticPass.boolType, 0, 0));
		}
	}

	public void visit(FactorCharacter Const) {
		Code.load(new Obj(Obj.Con, "$", Const.struct, Const.getC1(), 0));
	}

	public void visit(TermMulop mulopTerm) {
		Mulop op = mulopTerm.getMulop();
		if (op instanceof Mul) {
			Code.put(Code.mul);
		} else if (op instanceof Div) {
			Code.put(Code.div);
		} else {
			// instanceof mod
			Code.put(Code.rem);
		}
	}

	@Override
	public void visit(DesignArr Designator) {
		SyntaxNode parent = Designator.getParent();
		if (DesignatorEqual.class != parent.getClass() && DesignatorInc.class != parent.getClass()
				&& DesignatorDec.class != parent.getClass() && ReadStatement.class != parent.getClass()) {
			Code.load(Designator.obj);
		}
	}

	public void visit(DesignNoArr Designator) {
		SyntaxNode parent = Designator.getParent();
		if (DesignatorEqual.class != parent.getClass() && FuncCallNoArgs.class != parent.getClass()
				&& FuncCallWithArgs.class != parent.getClass() && FactorProc.class != parent.getClass()
				&& FactorFunc.class != parent.getClass() && ReadStatement.class != parent.getClass()) {
			Code.load(Designator.obj);
		}
	}

	public void visit(FactorNewArrayElem newArrayElem) {
		Code.put(Code.newarray);
		if (newArrayElem.getType().struct == Tab.charType)
			Code.put(0);
		else
			Code.put(1);
	}

	// iz Designator-a
	@Override
	public void visit(FuncCallNoArgs FuncCall) {
		Obj functionObj = FuncCall.getDesignator().obj;
		int offset = functionObj.getAdr() - Code.pc;
		Code.put(Code.call);
		Code.put2(offset);
		if (functionObj.getType() != Tab.noType) {
			Code.put(Code.pop);
		}
	}

	// iz Designator-a
	public void visit(FuncCallWithArgs FuncCall) {
		Obj functionObj = FuncCall.getDesignator().obj;
		int offset = functionObj.getAdr() - Code.pc;

		if (functionObj.getName().equals("len")) {
			Code.put(Code.arraylength);
			return;
		}

		if (functionObj.getName().equals("chr") || functionObj.getName().equals("ord")) {
			return;
		}

		Code.put(Code.call);
		Code.put2(offset);
		if (functionObj.getType() != Tab.noType) {
			Code.put(Code.pop);
		}
	}

	// iz Factor-a
	public void visit(FactorProc FuncCall) {
		Obj functionObj = FuncCall.getDesignator().obj;
		int offset = functionObj.getAdr() - Code.pc;
		Code.put(Code.call);
		Code.put2(offset);
	}

	public void visit(FactorFunc FuncCall) {
		Obj functionObj = FuncCall.getDesignator().obj;
		int offset = functionObj.getAdr() - Code.pc;

		if (functionObj.getName().equals("len")) {
			Code.put(Code.arraylength);
			return;
		}

		if (functionObj.getName().equals("chr") || functionObj.getName().equals("ord")) {
			return;
		}

		Code.put(Code.call);
		Code.put2(offset);
	}

	@Override
	public void visit(PrintStatement PrintStmt) {
		int kind = PrintStmt.getExprExpanded().struct.getKind();
		boolean secondPar = PrintStmt.getExprExpanded() instanceof ExprExpanded2;
		if (kind == Struct.Char) {
			if (secondPar) {
				Code.loadConst(((ExprExpanded2) PrintStmt.getExprExpanded()).getN2());
			} else {
				Code.put(Code.const_1);
			}
			Code.put(Code.bprint);
		} else {
			if (secondPar) {
				Code.loadConst(((ExprExpanded2) PrintStmt.getExprExpanded()).getN2());
			} else {
				Code.put(Code.const_5);
			}
			Code.put(Code.print);
		}
	}

	public void visit(ReadStatement ReadStmt) {
		int kind = ReadStmt.getDesignator().obj.getKind();
		if (kind == Struct.Char) {
			Code.put(Code.bread);
		} else {
			Code.put(Code.read);
		}
		Code.store(ReadStmt.getDesignator().obj);
	}

	public void visit(MinusTermExp minusTerm) {
		Code.put(Code.neg);
	}

	@Override
	public void visit(ExprAddop AddExpr) {
		if (AddExpr.getAddop() instanceof Plus) {
			Code.put(Code.add);
		} else {
			// instanceof Minus
			Code.put(Code.sub);
		}
	}
}
