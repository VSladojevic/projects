/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package control;

import beans.Company;
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
public class adminCompanies {

    private HibernateUtil helper;
    private Session session;
    public User user = Controler.user;
    List<Company> companies = new ArrayList<Company>();

    Company nova = new Company();
    boolean show = false;

    public boolean isShow() {
        return show;
    }

    public void setShow(boolean show) {
        this.show = show;
    }

    public void addCompany(RowEditEvent event) {
        Company com = (Company) event.getObject();

        session.save(com);
        session.getTransaction().commit();

        ExternalContext ec = FacesContext.getCurrentInstance().getExternalContext();
        HttpServletRequest req=(HttpServletRequest)ec.getRequest();
        try {
            ec.redirect(req.getRequestURI());
        } catch (IOException ex) {
            Logger.getLogger(adminCompanies.class.getName()).log(Level.SEVERE, null, ex);
        }

    }

    public Company getNova() {
        return nova;
    }

    public void setNova(Company nova) {
        this.nova = nova;
    }

    @PostConstruct
    public void loadUsers() {
        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        Query q = session.createQuery("from Company");
        companies = (List<Company>) q.list();

    }

    public void onRowEdit(RowEditEvent event) {
        Company c = (Company) event.getObject();

        session.update(c);
        session.getTransaction().commit();
    }

    public void onRowCancel(RowEditEvent event) {

    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public List<Company> getCompanies() {
        return companies;
    }

    public void setCompanies(List<Company> companies) {
        this.companies = companies;
    }

}
