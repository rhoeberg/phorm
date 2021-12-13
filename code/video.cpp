void TestVideo()
{
	const char *filename = "assets/bjork.mpg";
	plm_t *plm = plm_create_with_filename(filename);
	if (!plm) {
		printf("Couldn't open file %s\n", filename);
		exit(1);
	}
	plm_set_audio_enabled(plm, FALSE);
	int w = plm_get_width(plm);
	int h = plm_get_height(plm);
	uint8_t *rgb_buffer = (uint8_t *)malloc(w * h * 3);
	char png_name[16];
	plm_frame_t *frame = NULL;

	if(!plm_seek(plm, 43.0, false)) {
		printf("couldn't seek frame\n");
		exit(1);
	}

	for (int i = 1; frame = plm_decode_video(plm); i++) {
		plm_frame_to_rgb(frame, rgb_buffer, w * 3);

		sprintf(png_name, "%06d.png", i);
		printf("Writing %s\n", png_name);
		stbi_write_png(png_name, w, h, 3, rgb_buffer, w * 3);
		break;
	}
	free(rgb_buffer);
	
    exit(0);
}
