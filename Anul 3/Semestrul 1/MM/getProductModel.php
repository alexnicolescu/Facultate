<?php
class ProductModel{
	private $name="";
	private $quantity="";
	private $genre="";
	private $images;
	private $description="";
	private $price=0;
	private $rating=0;
	private $author="";
	private $filename="";
	private $id=0;

	public function __construct($id)
	{
		$this->id=$id;
	}

	private function conectare()
	{

		$link=new mysqli("localhost","admin","admin","librarieonline");
		if($link->connect_error)
		{
			die("Connection failed: ". $link->connect_error);
		}
		return $link;
	}

	public function getDetails()
	{
		$sql="SELECT * FROM products WHERE id=? LIMIT 1";
		$link=$this->conectare();
		$statement=$link->prepare($sql);
		$statement->bind_param('i',$this->id);
		$statement->execute();
		$result=$statement->get_result();
		$products=$result->fetch_all(MYSQLI_ASSOC);
		$result->close();
		$statement->close();
		$product=$products[0];
        $this->name=$product['name'];
		$this->author=$product['author'];
		$this->quantity=$product['quantity'];
		$this->genre=$product['genre'];
		$this->description=$product['description'];
		$this->price=$product['price'];
		$this->rating=$product['rating'];
		$this->filename=$product['filename'];
        $link->close();
		
	}
    public function getName()
    {
        return $this->name;
    }
    public function getId()
    {
        return $this->id;
    }
	public function getQuantity()
	{
		return $this->quantity;
	}
	public function getGenre()
	{
		return $this->genre;
	}
	public function getDescription()
	{
		return $this->description;
	}
    public function getPrice()
    {
        return $this->price;
    }
    public function getRating()
    {
        return $this->rating;
    }
    public function getAuthor()
    {
        return $this->author;
    }
    public function getFilename()
    {
        return $this->filename;
    }
	public function updateDB($newText)
	{
        $link=$this->conectare();
		$sql="UPDATE products SET description=? WHERE id=?";
		$statement=$link->prepare($sql);
		$statement->bind_param("si",$newText,$this->id);
		$statement->execute();
		$statement->close();
		$link->close();
	}
    public function updateDBprice($newText)
    {
        $link=$this->conectare();
        $sql="UPDATE products SET price=? WHERE id=?";
        $statement=$link->prepare($sql);
        $statement->bind_param("di",$newText,$this->id);
        $statement->execute();
        $statement->close();
        $link->close();
    }
    public function updateDBname($newText)
    {
        $link=$this->conectare();
        $sql="UPDATE products SET name=? WHERE id=?";
        $statement=$link->prepare($sql);
        $statement->bind_param("si",$newText,$this->id);
        $statement->execute();
        $statement->close();
        $link->close();
    }
    public  function updateDBauthor($newText)
    {
        $link=$this->conectare();
        $sql="UPDATE products SET author=? WHERE id=?";
        $statement=$link->prepare($sql);
        $statement->bind_param("si",$newText,$this->id);
        $statement->execute();
        $statement->close();
        $link->close();
    }
    public  function updateDBquantity($newText)
    {
        $link=$this->conectare();
        $sql="UPDATE products SET quantity=? WHERE id=?";
        $statement=$link->prepare($sql);
        $statement->bind_param("ii",$newText,$this->id);
        $statement->execute();
        $statement->close();
        $link->close();
    }
    public  function updateDBgenre($newText)
    {
        $link=$this->conectare();
        $sql="UPDATE products SET genre=? WHERE id=?";
        $statement=$link->prepare($sql);
        $statement->bind_param("si",$newText,$this->id);
        $statement->execute();
        $statement->close();
        $link->close();
    }
}
class FictionIdModel{
    private $name="";
    private $quantity="";
    private $genre="";
    private $images;
    private $description="";
    private $price=0;
    private $rating=0;
    private $author="";
    private $filename="";
    private $id=0;
    public function __construct($name)
    {
        $this->name=$name;
    }
    protected function conectare()
    {
        $link = new mysqli("localhost", "admin", "admin", "librarieonline");
        if ($link->connect_error) {
            die("Connection failed: " . $link->connect_error);
        }
        return $link;
    }
    public function getDetails()
    {
        $sql="SELECT * FROM products WHERE name=? LIMIT 1";
        $link=$this->conectare();
        $statement=$link->prepare($sql);
        $statement->bind_param('s',$this->name);
        $statement->execute();
        $result=$statement->get_result();
        $products=$result->fetch_all(MYSQLI_ASSOC);
        $result->close();
        $statement->close();
        $product=$products[0];
        $this->id=$product['id'];
        $this->author=$product['author'];
        $this->quantity=$product['quantity'];
        $this->genre=$product['genre'];
        $this->description=$product['description'];
        $this->price=$product['price'];
        $this->rating=$product['rating'];
        $this->filename=$product['filename'];
        $link->close();

    }
    public function getId()
    {
        return $this->id;
    }
    public function search()
    {
       return $this->id;
    }
}