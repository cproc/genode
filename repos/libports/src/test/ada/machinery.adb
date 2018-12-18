pragma Ada_2012;

package body Machinery is

   procedure Initialize (Machinery : out Machinery_Type) is
   begin
      Machinery := ( Temperature => 25 );
   end Initialize;

   function Temperature (Machinery : Machinery_Type) return Temperature_Type is
   begin
      return Machinery.Temperature;
   end Temperature;

   procedure Heat_Up (Machinery : in out Machinery_Type) is
   begin
      Machinery.Temperature := 77;
   end Heat_Up;

end Machinery;
