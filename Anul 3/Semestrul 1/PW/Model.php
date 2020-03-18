<?php
	class Model{
		protected $username="";
		private $password="";
		public function __construct($username,$password)
		{
			$this->username=$username;
			$this->password=$password;
			$this->username=stripcslashes($username);
			$this->password=stripcslashes($password);
			$link=$this->conectare();
			$this->username=mysqli_real_escape_string($link,$username);
			$this->password=mysqli_real_escape_string($link,$password);
		}

		protected function conectare()
		{
			$link=new mysqli("localhost","admin","admin","librarieonline");
			if($link->connect_error)
			{
				die("Connection failed: ". $link->connect_error);
			}
			return $link;
		}

		public function searchUser()
		{
			$link=$this->conectare();
			$sql="SELECT * FROM users WHERE username = '$this->username' AND password= MD5('$this->password')";
			$result=$link->query($sql);
			if($result->num_rows >0)
			{
				while($row=$result->fetch_assoc())
				{
					if($row['username'] == $this->username && $row['password']== md5($this->password))
					{
						return 0;
					}
					else
						return 1;
				}
			}
			else
				return -1;
		}
}
class ModelRating extends Model{

	    private $nr=0;
	public function __construct($username)
	{
		$this->username=$username;
	}


	public function getRating($name)
	{
		$link=$this->conectare();
		$nameQuery="SELECT * FROM users WHERE username=?";
		$sql=$link->prepare($nameQuery);
		$sql->bind_param('s',$this->username);
		$sql->execute();
		$result=$sql->get_result();
		$sql->close();
		if($result->num_rows>0)
		{
			$row=$result->fetch_assoc();
			return $row[$name];
		} 
	}

	public function updateRating($name,$rate)
    {
        $link=$this->conectare();
        $nameQuery="UPDATE users SET `$name`=? WHERE username=?";
        $sql=$link->prepare($nameQuery);
        $sql->bind_param('is',$rate,$this->username);
        $sql->execute();
        $sql->close();
        $link->close();
    }

    public function getRates($name)
    {

        $link=$this->conectare();
        $nameQuery="SELECT * FROM users";
        $result=$link->query($nameQuery);
        $sum=0;
        if($result->num_rows>0)
        {
            while($row=$result->fetch_assoc())
            {
                if($row[$name]!=NULL)
                {
                    $sum=$sum+$row[$name];
                    $this->nr=$this->nr+1;
                }
            }
        }
        $link->close();
        return $sum;
    }

    public function getNr()
    {
        return $this->nr;
    }

    public function deleteRow($name)
    {
        $link=$this->conectare();
        $nameQuery="ALTER TABLE users DROP COLUMN `$name`";
        $link->query($nameQuery);
        $link->close();
    }



}