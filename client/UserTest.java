package com.morgan;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

public class UserTest extends JFrame {

	public UserTest(String title) {
		super(title);
	}

	public static void main(String[] args) {
		JFrame friendList = new JFrame("Friend List");
		friendList.setSize(150, 500);
		friendList.setResizable(false);
		friendList.setLocation(500, 250);
		friendList.setLayout(new BorderLayout());
		JPanel panel = new JPanel();
		String[] strs = { "Peter", "Anne", "Ken", "Sue", "John" };
		final JList list = new JList(strs);
		Font f = new Font(Font.SANS_SERIF, Font.BOLD, 30);
		list.setFont(f);
		panel.add(list);
		friendList.add(panel);
		friendList.setVisible(true);

		// 列表监听事件

		list.addMouseListener(new MouseListener() {

			@Override
			public void mouseReleased(MouseEvent e) {
				// TODO Auto-generated method stub

			}

			@Override
			public void mousePressed(MouseEvent e) {
				// TODO Auto-generated method stub

			}

			@Override
			public void mouseExited(MouseEvent e) {
				// TODO Auto-generated method stub

			}

			@Override
			public void mouseEntered(MouseEvent e) {
				// TODO Auto-generated method stub

			}

			@Override
			public void mouseClicked(MouseEvent e) {
				// TODO Auto-generated method stub
				String str = list.getSelectedValue().toString();
				System.out.println(str);

				JFrame chat = new JFrame("Chatting with " + str);
				JTextArea jTextArea = new JTextArea();
				jTextArea = new JTextArea();
				jTextArea.setLineWrap( true );
				jTextArea.setEditable( false );
				jTextArea.setLineWrap( true );
				
				
				JButton btnSend = new JButton("send");
				JEditorPane e2 = new JEditorPane();
				
				JPanel panel = new JPanel();
				panel.setLayout(new FlowLayout(FlowLayout.CENTER));
				
				panel.add(new JScrollPane( jTextArea));
				panel.add(btnSend);
				panel.add(e2);
				chat.add(panel);

				chat.setSize(400, 400);
				//chat.setResizable(false);
				chat.setLocation(500, 250);
				chat.setVisible(true);
			}
		});

	}
}
