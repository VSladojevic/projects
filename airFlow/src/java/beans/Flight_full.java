/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package beans;

import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author Vladimir
 */
public class Flight_full {
    Flight basic;
    Airport dep;
    Airport arr; 
    Airplane_model plane;
    
    String dep_gate_name;  
    String arr_gate_name; 
    
    List<Radar_center> radars=new ArrayList<Radar_center>();

    public List<Radar_center> getRadars() {
        return radars;
    }

    public void setRadars(List<Radar_center> radars) {
        this.radars = radars;
    }

    
    
    public Airport getDep() {
        return dep;
    }

    public void setDep(Airport dep) {
        this.dep = dep;
    }

    public Airport getArr() {
        return arr;
    }

    public void setArr(Airport arr) {
        this.arr = arr;
    }

    public Airplane_model getPlane() {
        return plane;
    }

    public void setPlane(Airplane_model plane) {
        this.plane = plane;
    }
    

    public Flight getBasic() {
        return basic;
    }

    public void setBasic(Flight basic) {
        this.basic = basic;
    }

    public String getDep_gate_name() {
        return dep_gate_name;
    }

    public void setDep_gate_name(String dep_gate_name) {
        this.dep_gate_name = dep_gate_name;
    }

    public String getArr_gate_name() {
        return arr_gate_name;
    }

    public void setArr_gate_name(String arr_gate_name) {
        this.arr_gate_name = arr_gate_name;
    }

    public Flight_full(Flight basic) {
        this.basic = basic;
    }

    
    
    public Flight_full() {
    }
    
    
    
}
