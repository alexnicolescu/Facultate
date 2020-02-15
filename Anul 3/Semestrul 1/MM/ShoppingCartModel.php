<?php
class ShoppingCartModel{
    private $name="";

    public function __construct($name)
    {
        $this->name=$name;
    }
    public function updateStock($quantity)
    {
        $link=$this->conectare();
        $nameQuery="SELECT quantity FROM products WHERE name=?";
        $sql=$link->prepare($nameQuery);
        $sql->bind_param('s',$this->name);
        $sql->execute();
        $result=$sql->get_result();
        $sql->close();
        if($result->num_rows>0)
        {
            $row=$result->fetch_assoc();
            $nr=$row['quantity']-$quantity;
            $nameQuery="UPDATE products SET quantity=? WHERE name=?";
            $sql=$link->prepare($nameQuery);
            $sql->bind_param('is',$nr,$this->name);
            $sql->execute();
            $sql->close();
        }
        $link->close;
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
}
