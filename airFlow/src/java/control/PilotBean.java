/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package control;

import javax.faces.bean.ManagedBean;
import javax.faces.bean.SessionScoped;
import beans.*;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.List;
import javax.faces.application.FacesMessage;
import javax.faces.context.FacesContext;
import org.hibernate.Query;
import org.hibernate.Session;

/**
 *
 * @author Vladimir
 */
@ManagedBean
@SessionScoped
public class PilotBean {

    public User user = Controler.user;
    private String license = "";
    private HibernateUtil helper;
    private Session session;
    private String kompanija = "";
    String poruka="ready";
    
    

    String newstatus = "";
    int km = 0;
    int speed = 0;

    public String getPoruka() {
        return poruka;
    }

    public void setPoruka(String poruka) {
        this.poruka = poruka;
    }

    
    
    public String getNewstatus() {
        return newstatus;
    }

    public void setNewstatus(String newstatus) {
        this.newstatus = newstatus;
    }

    public int getKm() {
        return km;
    }

    public void setKm(int km) {
        this.km = km;
    }

    public int getSpeed() {
        return speed;
    }

    public void setSpeed(int speed) {
        this.speed = speed;
    }

    Flight_full selected = new Flight_full();

    public Flight_full getSelected() {
        return selected;
    }

    public void setSelected(Flight_full selected) {
        this.selected = selected;
    }

    public String getKompanija() {
        return kompanija;
    }

    public void setKompanija(String kompanija) {
        this.kompanija = kompanija;
    }

    public String changeCompany() {

        for (Company c : RegBean.kompanije) {
            if (c.getName().equals(kompanija)) {
                session = helper.getSessionFactory().openSession();
                session.beginTransaction();
                user.setCompany(c.getId());
                session.update(user);
                session.getTransaction().commit();
                break;
            }
        }
        return "pilot";
    }

    public String addLicense() {
        session = helper.getSessionFactory().openSession();
        session.beginTransaction();

        License l = new License();
        l.setNumber(license);
        l.setAirplane_license(license.substring(0, 2));
        l.setPilot_id(user.getId());

        session.save(l);
        session.getTransaction().commit();

        return "pilot";
    }

    public String updateStatus() {
        

        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        Flight let = selected.getBasic();
        let.setStatus(newstatus);

        session.update(let);
        session.getTransaction().commit();

        return "pilot";
    }

    public String updateTime() {
        poruka="";
        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        Flight let = selected.getBasic();
        Query q = session.createQuery("from Radar_center where updated=0 and flight_id=" + let.getId());
        List<Radar_center> results = (List<Radar_center>) q.list();
        int i = results.size();
        if (i > 0) {
            
            Radar_center r=results.get(0);
            r.setUpdated(1);
            
            double sati = km / speed;
            long millis = (long) sati * 60 * 60 * 1000;
            java.sql.Timestamp sad = new Timestamp(System.currentTimeMillis());
            sad.setTime(sad.getTime() + millis);
            let.setExpected_arrival(sad);
            
            session.update(r);
            session.update(let);
            session.getTransaction().commit();

            return "pilot";
        }
        poruka="There is no more radar centers for this flight";
        return "";
    }

    public void openFlight(Flight_full let) {
        try {
            selected = let;
            FacesContext.getCurrentInstance().getExternalContext().redirect("pilot_update_flight.xhtml");

        } catch (Exception e) {
        }
    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public String getLicense() {
        return license;
    }

    public void setLicense(String license) {
        this.license = license;
    }

}
