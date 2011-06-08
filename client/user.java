package com.morgan;

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;

public class User extends JFrame{
	private static String userName="morgan";
	private static String passwd="yuan";
	
	public User(String title){
		super(title);
	}
	
	public static void main(String[] args){
		//设计登录界面
		final JFrame login = new JFrame("Login");
		login.setSize(300,200);
		login.setResizable(false);
		login.setLocation(500, 250);
		JPanel panel1 = new JPanel();
		JPanel panel2 = new JPanel();
		JPanel panel3 = new JPanel();
		login.setLayout(new GridLayout(3, 2));
		JLabel lblUser = new JLabel("User name: ");
		JLabel lblPwd = new JLabel("Password: ");
		final JTextField tfUser = new JTextField(10);
		final JPasswordField pfPwd = new JPasswordField(10);
		pfPwd.setEchoChar('*');
		JButton btnCon = new JButton("Confirm");
		JButton btnReg = new JButton("Regist");
		JButton btnCancel = new JButton("Cancel");
		
		panel1.add(lblUser);
		panel1.add(tfUser);
		panel2.add(lblPwd);
		panel2.add(pfPwd);
		panel3.add(btnCon);
		panel3.add(btnReg);
		panel3.add(btnCancel);
		
		login.add(panel1);
		login.add(panel2);
		login.add(panel3);
		
		login.setVisible(true);
		
		//编辑确认按钮事件
		btnCon.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				String msg = "hello";
				String inputUsername = tfUser.getText().toLowerCase();
				//用pfPwd.getPassword()有问题？
				String inputPassword = pfPwd.getText().trim();
				
				if(inputUsername.isEmpty()||inputPassword.isEmpty()){
					msg = "UserName or Password must not be empty.";
					JOptionPane.showMessageDialog(null, msg);
				}
				else if(inputUsername.length()>20||inputPassword.length()>20){
					msg = "UserName or Password must not be longer than 20 chars.";
					JOptionPane.showMessageDialog(null, msg);
				}
				//System.out.println(inputUsername+"   "+inputPassword);
				else{
					login.dispose();
					
					JFrame friendList = new JFrame("Friend List");
					friendList.setSize(300,300);
					friendList.setVisible(true);
					friendList.setLocation(500, 250);
					friendList.setResizable(false);
					
					//朋友列表的处理
					
				}
				
			
			    
			}
		});
		
		//编辑注册按钮事件
		btnReg.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				final JFrame regist = new JFrame("Regist");
				regist.setSize(300,300);
				login.setVisible(false);
				regist.setVisible(true);
				regist.setLocation(500, 250);
				regist.setResizable(false);
				
				JPanel p1 = new JPanel();
				JPanel p2 = new JPanel();
				JPanel p3 = new JPanel();
				JPanel p4 = new JPanel();
				regist.setLayout(new GridLayout(4, 2));
				JLabel l1 = new JLabel("User name: ");
				JLabel l2 = new JLabel("Password: ");
				JLabel l3 = new JLabel("Confirm pwd: ");
				final JTextField t1 = new JTextField(10);
				final JPasswordField pf1 = new JPasswordField(10);
				final JPasswordField pf2 = new JPasswordField(10);
				JButton b = new JButton("Regist");
				
				p1.add(l1);
				p1.add(t1);
				p2.add(l2);
				p2.add(pf1);
				p3.add(l3);
				p3.add(pf2);
				p4.add(b);
				
				regist.add(p1);
				regist.add(p2);
				regist.add(p3);
				regist.add(p4);
				
				//因为取消注册而关闭注册页面时回到登录页面
				regist.addWindowListener(new java.awt.event.WindowAdapter() {
					   public void windowClosing(java.awt.event.WindowEvent e) {
					    login.setVisible(true);
					    }
					  }); 
				
				//编辑注册确认按钮事件
				b.addActionListener(new ActionListener() {
					
					@Override
					public void actionPerformed(ActionEvent arg0) {
						// TODO Auto-generated method stub
						String str = "hello";
						String inputUsername = t1.getText().toLowerCase();
						String inputPassword = pf1.getText().trim();
						String inputConfrimPassword =  pf2.getText().trim();
						if(inputUsername.isEmpty()||inputPassword.isEmpty()||inputConfrimPassword.isEmpty()){
							str = "UserName or Password must not be empty.";
							JOptionPane.showMessageDialog(null, str);
						}
						else if(inputUsername.length()>20||inputPassword.length()>20){
							str = "UserName or Password must not be longer than 20 chars.";
							JOptionPane.showMessageDialog(null, str);
						}
						else if(!inputPassword.equals(inputConfrimPassword)){
							str = "Fail to confirm password";
							JOptionPane.showMessageDialog(null, str);
						}else{
							str = "Regist sucessful! Welcome.";
							JOptionPane.showMessageDialog(null, str);
							regist.dispose();
							
							
						}
					}
				});
			}
		});
		
		//编辑取消按钮事件
		btnCancel.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				System.exit(0);
			}
		});
		
		
	}
}