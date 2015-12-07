package edu.thu.xface.adapter;

/**
 * 
 * user info
 * 
 * @author hujiawei
 * 
 */
public class UserInfo {

	private int count;// pictures count
	private int userid;// user id
	private String name;// user name

	public UserInfo() {
	}

	public UserInfo(int count, int userid, String name) {
		this.count = count;
		this.userid = userid;
		this.name = name;
	}

	@Override
	public boolean equals(Object o) {//for "contains"!
		if (o instanceof UserInfo) {
			UserInfo userInfo = (UserInfo) o;
			if (userInfo.getName().equalsIgnoreCase(this.name)) {
				return true;
			}
		}
		return false;
	}

	public int getCount() {
		return count;
	}

	public void setCount(int count) {
		this.count = count;
	}

	public int getUserid() {
		return userid;
	}

	public void setUserid(int userid) {
		this.userid = userid;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

}
