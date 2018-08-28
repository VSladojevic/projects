/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package control;

import beans.HibernateUtil;
import beans.User;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.SessionScoped;

import java.util.*;
import javax.annotation.PostConstruct;
import javax.faces.context.FacesContext;
import org.hibernate.Query;
import org.hibernate.Session;
import org.primefaces.event.CellEditEvent;

/**
 *
 * @author Vladimir
 */
@ManagedBean
@SessionScoped
public class AdminBean {

    private HibernateUtil helper;
    private Session session;
    public User user = Controler.user;
    List<User> users = new ArrayList<User>();

    @PostConstruct
    public void loadUsers() {
        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        Query q = session.createQuery("from User where Admin=0");
        users = (List<User>) q.list();

    }

    public void onCellEdit(CellEditEvent event) {
        
        FacesContext context = FacesContext.getCurrentInstance();
        User row = context.getApplication().evaluateExpressionGet(context, "#{u}", User.class);
        
        Object oldValue = event.getOldValue();
        Object newValue = event.getNewValue();

        int nVal = (int) newValue;
        
        row.setApproved(nVal);
        session.update(row);
        session.getTransaction().commit();
        

    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public List<User> getUsers() {
        return users;
    }

    public void setUsers(List<User> users) {
        this.users = users;
    }

}
