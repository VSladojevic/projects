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
public class Reservation {
    int code;
    int flight_id;
    Integer flight_id2;
    String name;
    String surname;

    public Reservation() {
    }

    public Integer getFlight_id2() {
        return flight_id2;
    }

    public void setFlight_id2(Integer flight_id2) {
        this.flight_id2 = flight_id2;
    }

  
    
    
    public int getCode() {
        return code;
    }

    public void setCode(int code) {
        this.code = code;
    }

    public int getFlight_id() {
        return flight_id;
    }

    public void setFlight_id(int flight_id) {
        this.flight_id = flight_id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getSurname() {
        return surname;
    }

    public void setSurname(String surname) {
        this.surname = surname;
    }
    
    
}
