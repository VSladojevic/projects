/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package control;

import beans.User;
import beans.License;
import beans.HibernateUtil;
import beans.User;
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
public class adminLicense {

    private HibernateUtil helper;
    private Session session;
    public User user = Controler.user;
    List<User> pilots = new ArrayList<User>();
    User selected;
    boolean show = false;

    String license_no;

    
    public void addLicense() {

        License l = new License();
        l.setPilot_id(selected.getId());
        l.setNumber(license_no);
        l.setAirplane_license(license_no.substring(0, 2));

        session.save(l);
        session.getTransaction().commit();

        ExternalContext ec = FacesContext.getCurrentInstance().getExternalContext();
        HttpServletRequest req = (HttpServletRequest) ec.getRequest();
        try {
            ec.redirect(req.getRequestURI());
        } catch (IOException ex) {
            Logger.getLogger(adminCompanies.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public String getLicense_no() {
        return license_no;
    }

    public void setLicense_no(String license_no) {
        this.license_no = license_no;
    }

    public boolean isShow() {
        return show;
    }

    public void setShow(boolean show) {
        this.show = show;
    }

    
    public void onRowSelect() {
        setShow(true);

    }

    public void onRowUnselect() {
        setShow(false);

    }

    public User getSelected() {
        return selected;
    }

    public void setSelected(User selected) {
        this.selected = selected;
    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public List<User> getPilots() {
        return pilots;
    }

    public void setPilots(List<User> pilots) {
        this.pilots = pilots;
    }

    @PostConstruct
    public void loadUsers() {
        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        Query q = session.createQuery("from User where Type='pilot'");
        pilots = (List<User>) q.list();

    }
}
