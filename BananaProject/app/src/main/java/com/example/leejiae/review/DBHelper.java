package com.example.leejiae.review;

import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.support.annotation.Nullable;

public class DBHelper extends SQLiteOpenHelper {
    public DBHelper(@Nullable Context context, @Nullable String name, @Nullable SQLiteDatabase.CursorFactory factory, int version) {
        super(context, name, factory, version);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL("CREATE TABLE MEMBER (ID TEXT not null primary key, PW TEXT not null, SCOREF INTEGER not null, SCORES INTEGER not null);");
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int i, int i1) {

    }

    public void createBasket(String ID){
        SQLiteDatabase db = getWritableDatabase();
        String tableName=ID+"_zzim";
        db.execSQL("CREATE TABLE '"+tableName+"' (storeName TEXT not null primary key, URL TEXT not null);'");
    }

    public void insert(String ID, String PW, int SF, int SS) {
        SQLiteDatabase db = getWritableDatabase();
        db.execSQL("INSERT INTO MEMBER VALUES('" + ID + "','" + PW + "','" + SF + "','" + SS + "');'");
        db.close();
    }

    public void insertBasket(String ID,String store,String storeURL){
        SQLiteDatabase db = getWritableDatabase();
        String tableName=ID+"_zzim";
        db.execSQL("INSERT INTO '"+tableName+"' VALUES('" + store + "','" + storeURL +"');'");
        db.close();
    }

    public void updatePW(String ID, String PW) {
        SQLiteDatabase db = getWritableDatabase();
        db.execSQL("UPDATE MEMBER SET PW='" + PW + "' WHERE ID='" + ID + "';'");
        db.close();
    }

    public void updateSF(String ID, int SF) {
        SQLiteDatabase db = getWritableDatabase();
        db.execSQL("UPDATE MEMBER SET SCOREF='" + SF + "' WHERE ID='" + ID + "';'");
        db.close();
    }

    public void updateSS(String ID, int SS) {
        SQLiteDatabase db = getWritableDatabase();
        db.execSQL("UPDATE MEMBER SET SCORES='" + SS + "' WHERE ID='" + ID + "';'");
        db.close();
    }

    public void delete(String ID) {
        SQLiteDatabase db = getWritableDatabase();
        db.execSQL("DELETE FROM MEMBER WHERE ID='" + ID + "';'");
        db.close();
    }

    public void deleteBasket(String ID,String storeName){
        SQLiteDatabase db = getWritableDatabase();
        String tableName=ID+"_zzim";
        db.execSQL("DELETE FROM '"+tableName+"' WHERE storeName='" + storeName + "';'");
        db.close();
    }

    public boolean login(String userID, String password) {
        SQLiteDatabase db = getReadableDatabase();
        Cursor cursor = db.rawQuery("SELECT * FROM MEMBER;", null);
        cursor.moveToFirst();
        while (!cursor.isAfterLast()) {
            String cID = cursor.getString(0);
            String cPW = cursor.getString(1);
            if (userID.equals(cID) && password.equals(cPW)) {
                cursor.close();
                return true;
            }
            cursor.moveToNext();
        }
        cursor.close();
        return false;
    }

    public boolean select(String userID) {
        SQLiteDatabase db = getReadableDatabase();
        Cursor cursor = db.rawQuery("SELECT * FROM MEMBER;", null);
        cursor.moveToFirst();
        while (!cursor.isAfterLast()) {
            String cID = cursor.getString(0);
            if (userID.equals(cID)) {
                cursor.close();
                return true;
            }
            cursor.moveToNext();
        }
        if (cursor.isAfterLast()) {
            cursor.close();
            return false;
        }
        cursor.close();
        return false;
    }

    public String[][] selectBasket(String ID){
        SQLiteDatabase db=getReadableDatabase();
        String tableName=ID+"_zzim";
        Cursor cursor=db.rawQuery("SELECT * FROM '"+tableName+"';'",null);
        String[][]arr=new String[2][cursor.getCount()];
        while (cursor.moveToNext()){
            arr[0][cursor.getPosition()]=cursor.getString(0);
            arr[1][cursor.getPosition()]=cursor.getString(1);
        }
        return arr;
    }

    public boolean selectStoreName(String ID,String sN){
        SQLiteDatabase db = getReadableDatabase();
        String tableName=ID+"_zzim";
        Cursor cursor = db.rawQuery("SELECT * FROM '"+tableName+"';'", null);
        while (cursor.moveToNext()) {
            String cSN = cursor.getString(0);
            if (sN.equals(cSN)) {
                cursor.close();
                return true;
            }
        }
        cursor.close();
        return false;
    }

    public int getCursorPoint(String ID){
        SQLiteDatabase db = getReadableDatabase();
        String tableName=ID+"_zzim";
        Cursor cursor = db.rawQuery("SELECT * FROM '"+tableName+"';'", null);
        return cursor.getCount();
    }
}