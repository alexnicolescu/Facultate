using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class BDStergere : Form
    {
        public BDStergere()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (Equals(txtCNP.Text, "") || !OnlyNumbers() || txtCNP.Text.Length != 13)
                MessageBox.Show("Introduceti un CNP valid");
            else
            {
                using (UserEntities context = new UserEntities())
                {
                    try {
                        tblUser2 obj = context.tblUser2.FirstOrDefault(r => r.CNP == txtCNP.Text);
                        if (obj != null)
                        {
                            context.tblUser2.Remove(obj);
                            context.SaveChanges();
                            MessageBox.Show("Stergere realizata cu succes");
                            Clear();
                        }
                        else
                            MessageBox.Show("Nu exista persoana cauta");
                        
                    }
                    catch(Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                    }
                    

                }
            }
        }
        private void Clear()//Goleste camp-urile din form
        {
             txtCNP.Text ="";
        }
        private Boolean OnlyNumbers()//Verifica daca CNP-ul este format doar in cifre
        {
            for (int i = 0; i < txtCNP.Text.Length; i++)
                if (txtCNP.Text[i] < '0' || txtCNP.Text[i] > '9')
                    return false;
            return true;
        }
    }
}
