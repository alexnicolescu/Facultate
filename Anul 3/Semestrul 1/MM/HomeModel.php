<?php
class HomeModel
{
    private $category = "";

    public function __construct($category)
    {
        $this->category = $category;
    }

    private function conectare()
    {
        $link = new mysqli("localhost", "admin", "admin", "librarieonline");
        if ($link->connect_error) {
            die("Connection failed: " . $link->connect_error);
        }
        return $link;
    }

    public function getCategories()
    {
        $nameQuery = "SELECT * FROM categories";
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
                $array[$n] = $row;

                $n = $n + 1;
            }
            return $array;
        }

        return NULL;

    }

    public function addCategory()
    {
        $link = $this->conectare();
        $sql = "INSERT INTO categories (category) VALUES (?)";
        $statement = $link->prepare($sql);
        $statement->bind_param('s', $this->category);
        $statement->execute();
        $statement->close();
        $link->close();
    }

    public function deleteCategory()
    {
        $link = $this->conectare();
        $nameQuery = "DELETE FROM categories WHERE category=?";
        $link = $this->conectare();
        $sql = $link->prepare($nameQuery);
        $sql->bind_param('s', $this->category);
        $sql->execute();
        $sql->close();
        $link->close();
    }
}

class FictionCategoryModel{
        private $category="";
    public function __construct($category)
    {
        $this->category=$category;
    }
    private function conectare()
    {
        $link = new mysqli("localhost", "admin", "admin", "librarieonline");
        if ($link->connect_error) {
            die("Connection failed: " . $link->connect_error);
        }
        return $link;
    }
    public function  deleteCategory()
    {
        $link=$this->conectare();
        $nameQuery="DELETE FROM products WHERE category=?";
        $link = $this->conectare();
        $sql = $link->prepare($nameQuery);
        $sql->bind_param('s', $this->category);
        $sql->execute();
        $sql->close();
        $link->close();
    }
    public function deleteCategoryFromGenres()
    {
        $link=$this->conectare();
        $nameQuery="DELETE FROM genres WHERE category=?";
        $link = $this->conectare();
        $sql = $link->prepare($nameQuery);
        $sql->bind_param('s', $this->category);
        $sql->execute();
        $sql->close();
        $link->close();
    }

}