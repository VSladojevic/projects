/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package control;

import beans.Flight_full;
import javax.faces.context.FacesContext;

import javax.faces.bean.ManagedBean;
import javax.faces.bean.SessionScoped;
import javax.faces.bean.ViewScoped;
/**
 *
 * @author Vladimir
 */
@ManagedBean
@SessionScoped
public class stewardDetails {

    Flight_full selected_flight;

    public Flight_full getSelected_flight() {
        return selected_flight;
    }

    public void setSelected_flight(Flight_full selected_flight) {
        this.selected_flight = selected_flight;
    }

   
    public void onRowSelect() {
        try {
            //inicMap();
            FacesContext.getCurrentInstance().getExternalContext().redirect("steward_flight_details.xhtml");

        } catch (Exception e) {
        }
    }

}
