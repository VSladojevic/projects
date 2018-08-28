/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package control;

import beans.HibernateUtil;
import beans.User;
import java.util.Date;
import java.util.List;
import javax.faces.application.FacesMessage;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ViewScoped;
import javax.faces.context.FacesContext;
import javax.validation.constraints.Pattern;
import org.hibernate.Query;
import org.hibernate.Session;

import java.util.regex.*;
import beans.Company;
import java.time.ZonedDateTime;
import java.util.ArrayList;

/**
 *
 * @author Vladimir
 */
@ManagedBean
@ViewScoped
public class RegBean {

    static List<Company> kompanije = new ArrayList<Company>();
    private static HibernateUtil helper;
    private static Session session;
    static List<String> kompanijeNazivi = new ArrayList<String>();

    static {
        kompanije = new ArrayList<Company>();
        kompanijeNazivi = new ArrayList<>();
        session = helper.getSessionFactory().openSession();
        session.beginTransaction();

        List<Company> results = null;
        try {
            Query q = session.createQuery("from Company");
            results = (List<Company>) q.list();

        } catch (Exception e) {
            e.printStackTrace();
        }
        if (results.isEmpty()) {
            kompanije = null;
        } else {
            kompanije = results;
            for (Company company : kompanije) {
                kompanijeNazivi.add(company.getName());
            }
        }
    }

    String errUsername = "";
    String errPassword = "";
    String errPassword2 = "";

    String ime;
    String prezime;
    Date rodj; // prebaciti u sql 

    @javax.validation.constraints.Pattern(regexp = "^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z]{2,3}$", message = "Invalid email.")
    String email;
    String kompanija;
    String username;

    String password;
    String password2;
    String tip;
    String male;

    java.util.regex.Pattern firstLetter = java.util.regex.Pattern.compile("^([a-z]|[A-Z])");
    java.util.regex.Pattern charConstraints = java.util.regex.Pattern.compile("(?=.*[a-z]{3,})(?=.*[A-Z])(?=.*[0-9])(?=.*[^A-Za-z0-9])");
    java.util.regex.Pattern repeat = java.util.regex.Pattern.compile("(.)\\1{2}");

    private boolean checkUsername() {
        session = helper.getSessionFactory().openSession();
        session.beginTransaction();

        List<User> results = null;
        try {
            Query q = session.createQuery("from User where username='" + username + "'");
            results = (List<User>) q.list();

        } catch (Exception e) {
            e.printStackTrace();
        }
        if (results.isEmpty()) {
            return true;
        } else {
            return false;
        }
    }

    public String submit() {
        errPassword = "";
        errPassword2 = "";
        errUsername = "";

        if (!checkUsername()) {
            //FacesContext.getCurrentInstance().addMessage("username", new FacesMessage("summary", "detail"));
            errUsername = "Username already exists";
            return "";
        }

        /*
        ZonedDateTime now = ZonedDateTime.now();
        ZonedDateTime yearsAgo = now.plusYears(-18);

        if (rodj.toInstant().isBefore(yearsAgo.toInstant())) {
            errUsername="Must be older than 18 years";
            return "";
        }
        */
        
        // provera password-a, pa zatim provera password==password2 
        if (!(password.length() <= 10 && password.length() >= 8)) {
            errPassword = "Password must be 8-10 characters";
            return "";
        }

        Matcher m1 = firstLetter.matcher(password);
        if (!m1.find()) {
            errPassword = "First charachter must be a letter";
            return "";
        }

        Matcher m2 = charConstraints.matcher(password);
        if (!m2.find()) {
            errPassword = "Must contain at least 3 lowercase letters, uppercase letter, numeric and special character ";
            return "";
        }

        Matcher m3 = repeat.matcher(password);
        if (m3.find()) {
            errPassword = "Only 2 same charachters consecutively";
            return "";
        }

        if (!password.equals(password2)) {
            errPassword2 = "Passwords must match";
            return "";
        }

        // insert into table users
        User u = new User(username, password);
        u.setName(ime);
        u.setSurname(prezime);
        if (male.equals("male")) {
            u.setGender('m');
        } else {
            u.setGender('f');
        }

        u.setEmail(email);
        u.setType(tip);

        java.util.Date utilDate = rodj;

        java.sql.Date sqlDate = new java.sql.Date(utilDate.getTime());
        u.setBirth(sqlDate);
        u.setFirst_login(0);
        u.setApproved(0);

        int idC = 1;
        for (Company c : kompanije) {
            if (kompanija.equals(c.getName())) {
                idC = c.getId();
                break;
            }
        }

        u.setCompany(idC);

        session.save(u);
        session.getTransaction().commit();

        return "regsuccess";
    }

    public String getErrUsername() {
        return errUsername;
    }

    public void setErrUsername(String errUsername) {
        this.errUsername = errUsername;
    }

    public List<Company> getKompanije() {
        return kompanije;
    }

    public void setKompanije(List<Company> kompanije) {
        this.kompanije = kompanije;
    }

    public List<String> getKompanijeNazivi() {
        return kompanijeNazivi;
    }

    public void setKompanijeNazivi(List<String> kompanijeNazivi) {
        this.kompanijeNazivi = kompanijeNazivi;
    }

    public String getErrPassword() {
        return errPassword;
    }

    public void setErrPassword(String errPassword) {
        this.errPassword = errPassword;
    }

    public String getErrPassword2() {
        return errPassword2;
    }

    public void setErrPassword2(String errPassword2) {
        this.errPassword2 = errPassword2;
    }

    public String getMale() {
        return male;
    }

    public void setMale(String male) {
        this.male = male;
    }

    public String getPassword2() {
        return password2;
    }

    public void setPassword2(String password2) {
        this.password2 = password2;
    }

    public String getIme() {
        return ime;
    }

    public void setIme(String ime) {
        this.ime = ime;
    }

    public String getPrezime() {
        return prezime;
    }

    public void setPrezime(String prezime) {
        this.prezime = prezime;
    }

    public Date getRodj() {
        return rodj;
    }

    public void setRodj(Date rodj) {
        this.rodj = rodj;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String mail) {
        this.email = mail;
    }

    public String getKompanija() {
        return kompanija;
    }

    public void setKompanija(String kompanija) {
        this.kompanija = kompanija;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getTip() {
        return tip;
    }

    public void setTip(String tip) {
        this.tip = tip;
    }

}
