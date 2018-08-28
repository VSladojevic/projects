/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package beans;

/**
 *
 * @author Vladimir
 */
public class Flight_result {

    Flight_full f1;
    Flight_full f2;

    

    public Flight_result(Flight_full f1) {
        this.f1 = f1;
    }

    public Flight_result(Flight_full f1, Flight_full f2) {
        this.f1 = f1;
        this.f2 = f2;
    }
    
    
    
    public Flight_full getF1() {
        return f1;
    }

    public void setF1(Flight_full f1) {
        this.f1 = f1;
    }

    public Flight_full getF2() {
        return f2;
    }

    public void setF2(Flight_full f2) {
        this.f2 = f2;
    }

    public Flight_result() {
    }
    
    
    
}
