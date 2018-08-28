/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package control;

import beans.Airport;
import beans.HibernateUtil;
import beans.User;
import beans.Terminal;
import com.sun.javafx.scene.control.skin.VirtualFlow;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.annotation.PostConstruct;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ViewScoped;
import javax.faces.context.ExternalContext;
import javax.faces.context.FacesContext;
import javax.servlet.http.HttpServletRequest;
import org.hibernate.Query;
import org.hibernate.Session;
import org.primefaces.event.RowEditEvent;

/**
 *
 * @author Vladimir
 */
@ManagedBean
@ViewScoped
public class adminAirports {

    private HibernateUtil helper;
    private Session session;
    public User user = Controler.user;
    List<Airport> airports = new ArrayList<Airport>();
    List<String> terminals = new ArrayList<String>();

    public List<String> getTerminals() {
        return terminals;
    }

    public void setTerminals(List<String> terminals) {
        this.terminals = terminals;
    }

    Airport novi = new Airport();

    Airport aero = new Airport();

    boolean showAir = true;
    boolean show = false;

    public boolean isShowAir() {
        return showAir;
    }

    public void setShowAir(boolean showAir) {
        this.showAir = showAir;
    }

    public boolean isShow() {
        return show;
    }

    public void setShow(boolean show) {
        this.show = show;
    }

    public Airport getAero() {
        return aero;
    }

    public void setAero(Airport aero) {
        this.aero = aero;
    }

    @PostConstruct
    public void loadAirports() {
        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        Query q = session.createQuery("from Airport");
        airports = (List<Airport>) q.list();

    }

    public String addTerminals() {
        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        for (String t : terminals) {
            Terminal ter = new Terminal();
            ter.setName(t);
            ter.setAirport_iata(novi.getIata());
            session.save(ter);
        }

        session.getTransaction().commit();
        return "admin";
    }

    public void addAirport(RowEditEvent event) {

        Airport a = (Airport) event.getObject();

        session.save(a);
        session.getTransaction().commit();
        aero = a;

        terminals = new ArrayList<String>();
        for (int i = 0; i < a.getBr_terminala(); i++) {
            terminals.add("");
        }
        setShow(true);
        setShowAir(false);

        /*
        ExternalContext ec = FacesContext.getCurrentInstance().getExternalContext();
        HttpServletRequest req = (HttpServletRequest) ec.getRequest();
        try {
            ec.redirect(req.getRequestURI());
        } catch (IOException ex) {
            Logger.getLogger(adminCompanies.class.getName()).log(Level.SEVERE, null, ex);
        }*/

 /*
        try {
            FacesContext.getCurrentInstance().getExternalContext().redirect("add_terminals.xhtml");
            FacesContext.getCurrentInstance().responseComplete();
        } catch (IOException ex) {
            Logger.getLogger(adminAirports.class.getName()).log(Level.SEVERE, null, ex);
        }*/
    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public List<Airport> getAirports() {
        return airports;
    }

    public void setAirports(List<Airport> airports) {
        this.airports = airports;
    }

    public Airport getNovi() {
        return novi;
    }

    public void setNovi(Airport novi) {
        this.novi = novi;
    }

}
