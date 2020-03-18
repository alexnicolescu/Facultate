<?php

class SignUpModel{
	private $username="";
	private $email="";
	private $password="";
	private $passwordConfirm="";
	public function __construct($username,$email,$password,$passwordConfirm)
	{
		$this->username=$username;
		$this->password=$password;
		$this->email=$email;
		$this->passwordConfirm=$passwordConfirm;

	}
	public function conectare()
	{
		$link=new mysqli("localhost","admin","admin","librarieonline");
		if($link->connect_error)
		{
			die("Connection failed: ". $link->connect_error);
		}
		return $link;
	}
	public function unicitateEmail()
	{
		$emailQuery="SELECT * FROM users WHERE email=? LIMIT 1";
		$link=$this->conectare();
		$sql=$link->prepare($emailQuery);
		$sql->bind_param('s',$this->email);
		$sql->execute();
		$result=$sql->get_result();
		$sql->close();
		$userCount=$result->num_rows;
		if($userCount>0)
		{
			return 1;
		}
		return 0;
	}

	public function unicitateNume()
    {
        $emailQuery="SELECT * FROM users WHERE username=? LIMIT 1";
        $link=$this->conectare();
        $sql=$link->prepare($emailQuery);
        $sql->bind_param('s',$this->username);
        $sql->execute();
        $result=$sql->get_result();
        $sql->close();
        $userCount=$result->num_rows;
        if($userCount>0)
        {
            return 1;
        }
        return 0;
    }

	public function insertUser()
	{
		$sql="INSERT INTO users (username,password,email) VALUES (?,?,?)";
		$link=$this->conectare();
		$stmt=$link->prepare($sql);
		$stmt->bind_param('sss',$this->username,$this->password,$this->email);
		if($stmt->execute())
		{
			$user_id=$link->insert_id;
			$stmt->close();
			return $user_id;
		}
		$stmt->close();
		return -1;
	}
}