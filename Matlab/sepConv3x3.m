function [ImgOut] = sepConv3x3(ImgIn,kernel,axOrLat)

PadImg = zeros(size(ImgIn,1)+2,size(ImgIn,2)+2);
%keyboard()
PadImg(2:end-1,2:end-1) = ImgIn;
PadImg(1,:) = PadImg(2,:);
PadImg(end,:) = PadImg(end-1,:);
PadImg(:,1) = PadImg(:,2);
PadImg(:,end) = PadImg(:,end-1);
ImgOut = zeros(size(ImgIn));
if axOrLat == 1
for i =  2:size(PadImg,1)-1
    for j =  2:size(PadImg,2)-1
        ImgOut(i-1,j-1) = kernel(1)*PadImg(i-1,j) + kernel(2)*PadImg(i,j)...
            + kernel(3)*PadImg(i+1,j);   
    end
end
else
for i =  2:size(PadImg,1)-1
    for j =  2:size(PadImg,2)-1
        ImgOut(i-1,j-1) = kernel(1)*PadImg(i,j-1) + kernel(2)*PadImg(i,j)...
            + kernel(3)*PadImg(i,j+1); 
    end
end
end