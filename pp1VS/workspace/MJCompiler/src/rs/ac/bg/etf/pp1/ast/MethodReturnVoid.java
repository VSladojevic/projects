// generated with ast extension for cup
// version 0.8
// 25/7/2018 12:51:17


package rs.ac.bg.etf.pp1.ast;

public class MethodReturnVoid extends MethodReturn {

    public MethodReturnVoid () {
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    public void childrenAccept(Visitor visitor) {
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("MethodReturnVoid(\n");

        buffer.append(tab);
        buffer.append(") [MethodReturnVoid]");
        return buffer.toString();
    }
}
