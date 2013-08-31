function sudoku(M)
%
%       M=[0,0,0,0,1,6,0,0,0;...
%          2,1,4,0,0,0,3,7,0;...
%          0,9,5,0,0,3,0,0,0;...
%          0,0,0,6,8,0,0,3,5;...
%          0,0,6,0,0,0,9,0,0;...
%          5,4,0,0,2,9,0,0,0;...
%          0,0,0,8,0,0,6,1,0;...
%          0,7,9,0,0,0,8,4,2;...
%          0,0,0,4,9,0,0,0,0]
%

%Find the First Zero 
firstId = min(find(M(:)==0));

%When there are no zeros left, we have a solution
if isempty(firstId)
    S = zeros([size(M),0]);
    S(:,:,size(S,3)+1) = M 
else
    % get the i,j that corresponds to firstId
    [i,j] = ind2sub([9,9],firstId);
   
    %arrange the numbers 1 through 9 in random order
    array = randperm(9);
    for index=1:9
        k = array(index);
        ii = (ceil(i/3)-1)*3+1;
        jj = (ceil(j/3)-1)*3+1;
        %these are the indices into the 3x3 block containing that cell
        mm = M(ii:ii+2,jj:jj+2);
        % M(i,:)==k will give a 1 for k in the row and zero for everything
        % else
        % The result will be all zeros if k is not in the row.
        % Check row, col and block
        if sum(M(i,:)==k)==0 & sum(M(:,j)==k)==0 & sum(mm(:)==k)==0 
            M(i,j) = k ; 
            %Call Recursively.  If next call "fails", this call picks up
            %where it left off.
            sudoku(M);
        end
    end
    % Can't find a good solution.  Pop back to the level that
    % Called this and try again...
end
