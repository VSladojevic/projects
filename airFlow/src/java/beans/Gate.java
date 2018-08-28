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
public class Gate {
    int id;
    String name;
    int terminal_id;
    String airport_iata;

    public Gate() {
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getTerminal_id() {
        return terminal_id;
    }

    public void setTerminal_id(int terminal_id) {
        this.terminal_id = terminal_id;
    }

    public String getAirport_iata() {
        return airport_iata;
    }

    public void setAirport_iata(String airport_iata) {
        this.airport_iata = airport_iata;
    }
    
    
    
    
}
