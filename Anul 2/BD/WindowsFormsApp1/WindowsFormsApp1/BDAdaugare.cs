using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace WindowsFormsApp1
{
    public partial class BDAdaugare : Form
    {
       // private string connectionString = @"Data Source=DESKTOP-2VBO84U\SQLEXPRESS;Initial Catalog=UserAdderDB;Integrated Security=True";
        public BDAdaugare()
        {
            InitializeComponent();
        }

        private void btnAdauga_Click(object sender, EventArgs e)
        {
            if (Equals(txtNume.Text.Trim(), "") || Equals(txtPrenume.Text.Trim(), "") || Equals(txtAdresa.Text.Trim(), "") || Equals(txtCNP.Text, ""))
                MessageBox.Show("Te rog completeaza campurile obligatorii");
            else
               if (txtCNP.Text.Length != 13)
                MessageBox.Show("CNP-ul trebuie sa contina 13 cifre");
            else
               if (!OnlyNumbers())
                MessageBox.Show("CNP-ul contine doar cifre");
            else
                if (Exists())
                MessageBox.Show("CNP-ul introdus exista deja");
            else
            using (UserEntities context = new UserEntities())//UserEntities e clasa care reprezinta baza de data(kind of)
            {
                    try
                    {
                        tblUser2 obj = new tblUser2
                        {
                            Nume = txtNume.Text.Trim(),
                            Prenume = txtPrenume.Text.Trim(),
                            CNP = txtCNP.Text,
                            Adresa = txtAdresa.Text.Trim()
                        };
                        context.tblUser2.Add(obj);
                        context.SaveChanges();
                        MessageBox.Show("Adaugare realizata cu succes");
                        Clear();
                    }
                    catch(Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                    }
                     
                    
            }
        }
        private void Clear()//Goleste camp-urile din form
        {
            txtNume.Text = txtPrenume.Text = txtCNP.Text = txtAdresa.Text = "";
        }
        private Boolean OnlyNumbers()//Verifica daca CNP-ul este format doar in cifre
        {
            for (int i = 0; i < txtCNP.Text.Length; i++)
                if (txtCNP.Text[i] < '0' || txtCNP.Text[i] > '9')
                    return false;
            return true;
        }
        private Boolean Exists()//Verifica daca CNP-ul introdus exista deja
        {
            using (UserEntities context = new UserEntities())
            {
                tblUser2 obj = context.tblUser2.FirstOrDefault(r => r.CNP == txtCNP.Text);
                if (obj != null)
                    return true;
                else
                    return false;
            }
        }
    }
}
