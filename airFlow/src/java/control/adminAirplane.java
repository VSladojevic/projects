/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package control;

import beans.Airplane;
import beans.Airplane_model;
import beans.HibernateUtil;
import beans.User;
import beans.Company;
import java.util.ArrayList;
import java.util.List;
import javax.annotation.PostConstruct;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ViewScoped;
import org.hibernate.Query;
import org.hibernate.Session;

/**
 *
 * @author Vladimir
 */
@ManagedBean
@ViewScoped
public class adminAirplane {

    private HibernateUtil helper;
    private Session session;
    public User user = Controler.user;

    Company komp;
    Airplane_model model;
    
    int kompId;
    int modelId;

    public int getKompId() {
        return kompId;
    }

    public void setKompId(int kompId) {
        this.kompId = kompId;
    }

    public int getModelId() {
        return modelId;
    }

    public void setModelId(int modelId) {
        this.modelId = modelId;
    }
    
    

    public String addPlane() {
        Airplane a=new Airplane();
        a.setModel_id(modelId);
        a.setOwner_company_id(kompId);
        
        session.save(a);
        session.getTransaction().commit();
        
        return "admin";
    }

    public Airplane_model getModel() {
        return model;
    }

    public void setModel(Airplane_model model) {
        this.model = model;
    }

    public Company getKomp() {
        return komp;
    }

    public void setKomp(Company komp) {
        this.komp = komp;
    }

    List<Airplane_model> modeli = new ArrayList<Airplane_model>();
    List<Company> kompanije = new ArrayList<Company>();

    @PostConstruct
    public void load() {

        session = helper.getSessionFactory().openSession();
        session.beginTransaction();
        Query q = session.createQuery("from Company");
        kompanije = (List<Company>) q.list();
        q = session.createQuery("from Airplane_model");
        modeli = (List<Airplane_model>) q.list();

    }

    public List<Airplane_model> getModeli() {
        return modeli;
    }

    public void setModeli(List<Airplane_model> modeli) {
        this.modeli = modeli;
    }

    public List<Company> getKompanije() {
        return kompanije;
    }

    public void setKompanije(List<Company> kompanije) {
        this.kompanije = kompanije;
    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

}
