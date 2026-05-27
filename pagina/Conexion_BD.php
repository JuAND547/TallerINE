<?php
class conexion_bd
{
    private $servidor;
    private $usuario;
    private $pass;
    private $base;
 public function __construct()
    {
        $this->servidor = 'localhost';
        $this->usuario  = 'root';
        $this->pass     = '';
        $this->base     = 'bd';

        $this->conexion = $this->conectar($this->servidor, $this->usuario, $this->pass, $this->base);
    }

        public function conectar($servidor, $usuario, $pass, $base)
    {
        $conexion = new mysqli($servidor, $usuario, $pass, $base);
        if ($conexion->connect_error) {
            die("No se pudo conectar a la base: " . $conexion->connect_error);
        }
        return $conexion;
    }

        public function cerrarConexion()
    {
        if ($this->conexion) {
            $this->conexion->close();
        }
    }

    public function mostrarTabla() {
        $sql = "SELECT * FROM jugador";
        $consulta = mysqli_query($this->conexion, $sql);
        return $consulta;
    }

    public function insertarDatos($nombre, $puntaje) {
        $sql = "INSERT INTO `jugador` (`id`, `nombre`, `puntaje`) VALUES (?,?);";
        $Stmt->bind_param("si", $nombre, $puntaje);
        $stmt->execute();
    }
    }