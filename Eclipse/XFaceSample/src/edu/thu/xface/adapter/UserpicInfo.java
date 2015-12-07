package edu.thu.xface.adapter;

/**
 * 
 * user pic info
 * 
 * @author hujiawei
 * 
 */
public class UserpicInfo {

	private int userid;// user id
	private String filename;// file name

	public UserpicInfo() {
	}

	public UserpicInfo(int userid, String filename) {
		this.userid = userid;
		this.filename = filename;
	}

	public int getUserid() {
		return userid;
	}

	public void setUserid(int userid) {
		this.userid = userid;
	}

	public String getFilename() {
		return filename;
	}

	public void setFilename(String filename) {
		this.filename = filename;
	}

}
