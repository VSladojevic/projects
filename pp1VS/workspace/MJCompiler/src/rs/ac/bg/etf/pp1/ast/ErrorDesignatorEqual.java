// generated with ast extension for cup
// version 0.8
// 25/7/2018 12:51:18


package rs.ac.bg.etf.pp1.ast;

public class ErrorDesignatorEqual extends DesignatorStatement {

    private Designator Designator;
    private ErrorAssingment ErrorAssingment;

    public ErrorDesignatorEqual (Designator Designator, ErrorAssingment ErrorAssingment) {
        this.Designator=Designator;
        if(Designator!=null) Designator.setParent(this);
        this.ErrorAssingment=ErrorAssingment;
        if(ErrorAssingment!=null) ErrorAssingment.setParent(this);
    }

    public Designator getDesignator() {
        return Designator;
    }

    public void setDesignator(Designator Designator) {
        this.Designator=Designator;
    }

    public ErrorAssingment getErrorAssingment() {
        return ErrorAssingment;
    }

    public void setErrorAssingment(ErrorAssingment ErrorAssingment) {
        this.ErrorAssingment=ErrorAssingment;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    public void childrenAccept(Visitor visitor) {
        if(Designator!=null) Designator.accept(visitor);
        if(ErrorAssingment!=null) ErrorAssingment.accept(visitor);
    }

    public void traverseTopDown(Visitor visitor) {
        accept(visitor);
        if(Designator!=null) Designator.traverseTopDown(visitor);
        if(ErrorAssingment!=null) ErrorAssingment.traverseTopDown(visitor);
    }

    public void traverseBottomUp(Visitor visitor) {
        if(Designator!=null) Designator.traverseBottomUp(visitor);
        if(ErrorAssingment!=null) ErrorAssingment.traverseBottomUp(visitor);
        accept(visitor);
    }

    public String toString(String tab) {
        StringBuffer buffer=new StringBuffer();
        buffer.append(tab);
        buffer.append("ErrorDesignatorEqual(\n");

        if(Designator!=null)
            buffer.append(Designator.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        if(ErrorAssingment!=null)
            buffer.append(ErrorAssingment.toString("  "+tab));
        else
            buffer.append(tab+"  null");
        buffer.append("\n");

        buffer.append(tab);
        buffer.append(") [ErrorDesignatorEqual]");
        return buffer.toString();
    }
}
