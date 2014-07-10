function color = GetNextColor()
global Colors
persistent index

if (isempty(Colors))
    Colors = CreateColors();
end

%init index
if (isempty(index))
    index=1;
end

color.background = Colors(index,1:3);
color.text = Colors(index,4:6);
color.backgroundDark = Colors(index,7:9);

%increment index or roll over
if(index >= length(Colors))
    index = 1;
else
    index = index + 1;
end
end

function colors = CreateColors()

colors = [1.000, 0.500, 0.000, 0.000, 0.000, 0.000, 0.400, 0.200, 0.150;...
          0.000, 0.500, 1.000, 1.000, 1.000, 1.000, 0.150, 0.200, 0.400;...
          0.500, 0.000, 1.000, 1.000, 1.000, 1.000, 0.200, 0.150, 0.400;...
          1.000, 0.000, 0.500, 0.000, 0.000, 0.000, 0.400, 0.150, 0.200;...
          0.000, 0.750, 0.750, 0.000, 0.000, 0.000, 0.150, 0.300, 0.300;...
          0.750, 0.000, 0.750, 1.000, 1.000, 1.000, 0.300, 0.150, 0.300;...
          0.750, 0.750, 0.000, 0.000, 0.000, 0.000, 0.300, 0.300, 0.150;...
          0.796, 0.000, 0.398, 1.000, 1.000, 1.000, 0.318, 0.150, 0.159;...
          0.597, 0.398, 0.000, 1.000, 1.000, 1.000, 0.239, 0.159, 0.150;...
          0.000, 0.796, 1.000, 0.000, 0.000, 0.000, 0.150, 0.318, 0.400;...
          1.000, 0.597, 0.398, 0.000, 0.000, 0.000, 0.400, 0.239, 0.159;...
          0.796, 0.597, 0.000, 1.000, 1.000, 1.000, 0.318, 0.239, 0.150;];
end