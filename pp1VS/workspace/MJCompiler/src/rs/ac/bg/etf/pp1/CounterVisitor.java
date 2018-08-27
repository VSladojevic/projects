package rs.ac.bg.etf.pp1;

import rs.ac.bg.etf.pp1.ast.FormParArr;
import rs.ac.bg.etf.pp1.ast.FormParVar;
import rs.ac.bg.etf.pp1.ast.Var;
import rs.ac.bg.etf.pp1.ast.VarArrayDecl;
import rs.ac.bg.etf.pp1.ast.VisitorAdaptor;

public class CounterVisitor extends VisitorAdaptor {
	
	protected int count;
	
	public int getCount() {
		return count;
	}
	
	public static class FormParamCounter extends CounterVisitor {

		@Override
		public void visit(FormParVar formParamDecl1) {
			count++;
		}
		
		public void visit(FormParArr formParamDecl1) {
			count++;
		}
	}
	
	public static class VarCounter extends CounterVisitor {		
		@Override
		public void visit(Var VarDecl) {
			count++;
		}
		
		public void visit(VarArrayDecl VarDecl) {
			count++;
		}
	}
}