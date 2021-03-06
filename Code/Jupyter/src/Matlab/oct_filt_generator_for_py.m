%-------------------------------------
% OCT-FILTER-GENERATOR
%-------------------------------------

% Calculate bands
%*************************************
fs = 48000;     % [Hz] desired sampling rate of device
fmin = 20;      % [Hz] minimal desired low-end
fmax = 20000;   % [Hz] minimal desired high-end

oct_bw_ratio = 1/3; % [1] desired octave-split

n_octs = log2(fmax/fmin);               % [1] number of octaves from fmin to fmax
n_thirds = ceil(n_octs/oct_bw_ratio);   % [1] number of octave divisions from fmin to fmax
n_coeffs_p_section = 6;
n_sections = 3;

fc = fmin*2.^((0:n_thirds) * oct_bw_ratio); % [Hz] center frequencies. 
                                            % "(0:n_bands)" means to iterate from 0 to n_bands 
                                            % and store result in vector
fl = fc*2^(-oct_bw_ratio/2);                % lower cutoffs @ -3dB (required by filter-designer)
fu = fc*2^(+oct_bw_ratio/2);                % upper cutoffs @ -3dB (required by filter-designer)


% Create csv-file
%*************************************
header = append('b10, b11, b12, a10, a11, a12, b20, b21, b22, a20, a21, a22, b30, b31, b32, a30, a31, a32', newline);

path = '../Datasets/Feature_extraction/th_oct_coeffs.csv';
fileID = fopen(path ,'w');
fprintf(fileID, header);
fclose(fileID);

fileID = fopen(path ,'a');
scalings = cell(n_thirds + 1,1);

% Loop through all filters and insert coefficients
%*************************************
for i_b = 0:n_thirds
    
    index = 1 + n_thirds - i_b;
    filter = gen_iir_osix_bp(fl(index), fu(index), fs); % filter creation
    scaler = 20*log10(  filter.ScaleValues(1) * ... % scales magnitude of filter back to 0dB
                        filter.ScaleValues(2) * ...
                        filter.ScaleValues(3) * ...
                        filter.ScaleValues(4));
    scalings{i_b+1} = scaler; 
    format long;
    
    [h,f] = freqz(filter.sosMatrix, 1024, fs);
    hold on;
    % plot(f, 20*log10(abs(h)));
    plot(f, 20*log10(abs(h)) + scaler );    % plot magnitude in absolute frequency and scaled to 0dB
    
    set(gca, 'XScale', 'log');  % show plot in log-scale
    ylim([-120 10]);              % only show up to a damping of -80dB
    %xlim([20 20000]);           % only show a bandwidth of 20-20000 Hz
    
    % The coefficients are stored in the array in the following order:
    % {b10, b11, b12, a11, a12, b20, b21, b22, a21, a22, ...}
    % based on style of ARM CMSIS DSP library for later ease of
    % compatibility
    for i_s = 1:n_sections
       for i_c = 1:n_coeffs_p_section
%            if i_c == 4 % the 4th coefficient (always 1) is not accepted
%                continue;
%            end
           if i_c == 5 || i_c == 6
               value = filter.sosMatrix(i_s, i_c) * (1); % dsp cmsis lib only works with sign-swapped denominator coeffs ???
           else
               value = filter.sosMatrix(i_s, i_c);
           end
           if(i_s == n_sections && i_c == n_coeffs_p_section)
               fprintf(fileID, '%.15f', value);
           else
               fprintf(fileID, '%.15f,    ', value);
           end    
       end
    end
    fprintf(fileID, newline);
    
    if ((mod(i_b, (1/oct_bw_ratio))) == 0) && ...   % only scan for each 3rd band
        ((i_b + 1) ~= 1) &&...                      % keep 1st band in 1st decimation group (only group with 4 members)
        (i_b < (4 * ((1/oct_bw_ratio))) + 1)        % only decimate 4 times, leave lower freqs at last decimation stage (fs/2^(4+1))
        fs = fs/2;                                  % finally, decimate
    end
end

% closing statement
fclose(fileID);

% create txt-file with scalings for later import
%'../STM32L476RG/Core/Inc/scalings.txt'
file_s = fopen('../Datasets/Feature_extraction/scalings.csv', 'w');
for n = 1:n_thirds+1
    if(n == n_thirds+1)
        content = string(scalings{n});
    else  
        content = string(scalings{n}) + ',';
    end    
    fprintf(file_s, content);
end
fclose(file_s);