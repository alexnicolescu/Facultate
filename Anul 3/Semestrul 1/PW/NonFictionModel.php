<?php


class NonFictionModel
{
    protected $name = "";
    protected $quantity = "";
    protected $genre = "";
    protected $lastInsertId;
    protected $price = 0;
    protected $author = "";
    protected $filename = "";
    protected $description="";

    public function __construct($name, $quantity, $genre, $price, $author, $filename,$description)
    {
        $this->name = $name;
        $this->quantity = $quantity;
        $this->genre = $genre;
        $this->price = $price;
        $this->author = $author;
        $this->filename = $filename;
        $this->description=$description;
        //$this->link=$this->connect();
    }

    protected function conectare()
    {
        $link = new mysqli("localhost", "admin", "admin", "librarieonline");
        if ($link->connect_error) {
            die("Connection failed: " . $link->connect_error);
        }
        return $link;
    }

    public function insertProduct()
    {
        $link = $this->conectare();
        $sql = "INSERT INTO products (name,author,quantity,genre,price,filename,description) VALUES (?,?,?,?,?,?,?)";
        $statement = $link->prepare($sql);
        $statement->bind_param('ssisdss', $this->name, $this->author, $this->quantity, $this->genre, $this->price, $this->filename,$this->description);
        $statement->execute();
        $this->lastInsertId = $link->insert_id;
        $statement->close();
        $link->close();
        $linkp = new PDO('mysql:host=localhost;dbname=librarieonline', 'admin', 'admin');
        $var = $this->name;
        $sql = "ALTER TABLE users ADD `$var` INT ";
        $linkp->query($sql);

        //$linkp->close();

    }

    public function unicitateNume()
    {
        $nameQuery = "SELECT * FROM products WHERE name=? LIMIT 1";
        $link = $this->conectare();
        $sql = $link->prepare($nameQuery);
        $sql->bind_param('s', $this->name);
        $sql->execute();
        $result = $sql->get_result();
        $sql->close();
        $count = $result->num_rows;
        if ($count > 0) {
            return 1;
        }
        return 0;

    }

}

class NonFictionModelSon extends NonFictionModel
{
    public function __construct($name)
    {
        $this->name = $name;
    }

    public function search()
    {
        $nameQuery = "SELECT * FROM products WHERE name=? LIMIT 1";
        $link = $this->conectare();
        $sql = $link->prepare($nameQuery);
        $sql->bind_param('s', $this->name);
        $sql->execute();
        $result = $sql->get_result();
        $sql->close();
        $userCount = $result->num_rows;
        if ($userCount > 0) {
            return 1;
        }
        return 0;

    }

    public function update($quantity)
    {
        $link = $this->conectare();
        $nameQuery = "SELECT quantity FROM products WHERE name=?";
        $sql = $link->prepare($nameQuery);
        $sql->bind_param('s', $this->name);
        $sql->execute();
        $result = $sql->get_result();
        $sql->close();
        if ($result->num_rows > 0) {
            $row = $result->fetch_assoc();
            $nr = $row['quantity'] + $quantity;
            $nameQuery = "UPDATE products SET quantity=? WHERE name=?";
            $sql = $link->prepare($nameQuery);
            $sql->bind_param('is', $nr, $this->name);
            $sql->execute();
            $sql->close();
        }
        $link->close();
    }

    public function delete()
    {
        $link = $this->conectare();
        $nameQuery = "DELETE FROM products WHERE name=?";
        $link = $this->conectare();
        $sql = $link->prepare($nameQuery);
        $sql->bind_param('s', $this->name);
        $sql->execute();
        $sql->close();
        $link->close();
    }

}

class NonFictionModelForGenreSearch extends NonFictionModel
{
    public function __construct($genre)
    {
        $this->genre = $genre;
    }

    public function search2()
    {
        if ($this->genre !== "All") {
            $nameQuery = "SELECT * FROM products WHERE genre=?";
            $link = $this->conectare();
            $sql = $link->prepare($nameQuery);
            $sql->bind_param('s', $this->genre);
            $sql->execute();
            $result = $sql->get_result();
            $sql->close();
            $userCount = $result->num_rows;
            if ($userCount > 0) {
                $array = NULL;
                $n = 0;
                while ($row = $result->fetch_assoc()) {
                    $array[$n] = $row;
                    $n = $n + 1;
                }
                return $array;
            }
        } else {
            $nameQuery = "SELECT * FROM products";
            $link = $this->conectare();
            $sql = $link->prepare($nameQuery);
            $sql->execute();
            $result = $sql->get_result();
            $sql->close();
            $userCount = $result->num_rows;
            if ($userCount > 0) {
                $array = NULL;
                $n = 0;
                while ($row = $result->fetch_assoc()) {
                    if ($row['genre'] != "Adventure" && $row['genre'] != "Romance" && $row['genre'] != "Comedy" && $row['genre'] != "Horror") {
                        $array[$n] = $row;

                        $n = $n + 1;
                    }
                }
                return $array;
            }
        }
        return NULL;
    }

    public function getEntries()
    {
        $nameQuery = "SELECT * FROM products";
        $link = $this->conectare();
        $sql = $link->prepare($nameQuery);
        $sql->execute();
        $result = $sql->get_result();
        $sql->close();
        $userCount = $result->num_rows;
        if ($userCount > 0) {
            $array = NULL;
            $n = 0;
            while ($row = $result->fetch_assoc()) {
                if ($row['genre'] != "Adventure" && $row['genre'] != "Romance" && $row['genre'] != "Comedy" && $row['genre'] != "Horror") {
                    $array[$n] = $row;

                    $n = $n + 1;
                }
            }
            return $array;
        }

    }
}

class NonFictionModelForRating extends NonFictionModel
{

    public function __construct($name)
    {
        $this->name = $name;
    }

    public function getRating()
    {
        $link = $this->conectare();
        $sql = "SELECT * FROM products WHERE name=? LIMIT 1";
        $statement = $link->prepare($sql);
        $statement->bind_param('s', $this->name);
        $statement->execute();
        $result = $statement->get_result();
        $statement->close();
        if ($result->num_rows > 0) {
            $row = $result->fetch_assoc();
            return $row['rating'];
        }
    }

    public function updateRating($rating)
    {
        $link = $this->conectare();
        $nameQuery = "UPDATE products SET rating=? WHERE name=?";
        $sql = $link->prepare($nameQuery);
        $sql->bind_param('ds', $rating, $this->name);
        $sql->execute();
        $sql->close();
        $link->close();
    }
}