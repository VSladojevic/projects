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
public class License {
    int id;
    String number;
    int pilot_id;
    String airplane_license;

    public License() {
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getNumber() {
        return number;
    }

    public void setNumber(String number) {
        this.number = number;
    }

    public int getPilot_id() {
        return pilot_id;
    }

    public void setPilot_id(int pilot_id) {
        this.pilot_id = pilot_id;
    }

    public String getAirplane_license() {
        return airplane_license;
    }

    public void setAirplane_license(String airplane_license) {
        this.airplane_license = airplane_license;
    }
    
    
}
